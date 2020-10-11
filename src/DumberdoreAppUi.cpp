/*
 ============================================================================
 Name		: DumberdoreAppUi.cpp
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : CDumberdoreAppUi implementation
 ============================================================================
 */

// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <CAknFileSelectionDialog.h>
#include <CAknMemorySelectionDialog.h>
#include <AknProgressDialog.h>
#include <aknnotewrappers.h>
#include <eikprogi.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>
#include <pathinfo.h>
#include <bautils.h>

#include <dumberdore_0xe3ca2de5.rsg>
#include "Dumberdore.hrh"
#include "Dumberdore.pan"
#include "DumberdoreApplication.h"
#include "DumberdoreAppUi.h"
#include "DumberdoreAppView.h"

#include <DumberDumb.h>

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CDumberdoreAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDumberdoreAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	// Create view object
	iAppView = CDumberdoreAppView::NewL(ClientRect());
	iDumbThreadProgressChecker = CPeriodic::NewL(CActive::EPriorityStandard);
	}
// -----------------------------------------------------------------------------
// CDumberdoreAppUi::CDumberdoreAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CDumberdoreAppUi::CDumberdoreAppUi()
  : iDumbThreadProgressChecker(NULL)
	{
	// No implementation required
	iDumbThread.SetHandle(0);
	}

// -----------------------------------------------------------------------------
// CDumberdoreAppUi::~CDumberdoreAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CDumberdoreAppUi::~CDumberdoreAppUi()
	{
	if (iAppView)
		{
		delete iAppView;
		iAppView = NULL;
		}
	
	if (iProgressDialog)
		iProgressDialog->ProcessFinishedL();
	
	if (iDumbThreadProgressChecker)
		delete iDumbThreadProgressChecker;
	
	if (iDumbThread.Handle() != 0)
		iDumbThread.Close();
	}

static TInt DumpRPKGThreadFunc(TAny *aUserData) {
	CTrapCleanup *cleanup = CTrapCleanup::New();
	
	TDumberRPKGThreadData *threadData = reinterpret_cast<TDumberRPKGThreadData*>(aUserData);
	TRAPD(result, Dumber::BuildRpkgL(threadData->iPath, threadData->iProgress, threadData->iShouldStop, threadData->iMode));
	
	delete cleanup;
	return result;
}

static TInt DumpRPKGProgressBarUpdater(TAny *aUserData) {
	CDumberdoreAppUi *appUi = reinterpret_cast<CDumberdoreAppUi*>(aUserData);
	return static_cast<TInt>(appUi->UpdateProgressBar());
}

void CDumberdoreAppUi::DialogDismissedL (TInt aButtonId) {
	if (aButtonId != -1) {
		return;
	}

	// Kill the dumping thread
	iDumbThreadData.iShouldStop = ETrue;
	
	if (iDumbThread.Handle() != 0) {
		TRequestStatus status;
		iDumbThread.Logon(status);
		
		// Wait for thread to done
		User::WaitForRequest(status);
		
		iDumbThread.Close();
		iDumbThread.SetHandle(0);
	} else {
		return;
	}

	CAknInformationNote *note = new (ELeave) CAknInformationNote();
	
	// Delete the file being dumped!
	RFs &theFs = iCoeEnv->FsSession();
	theFs.Delete(iDumbThreadData.iPath);

	HBufC* dumpCancelledText = iEikonEnv->AllocReadResourceLC(R_DUMP_CANCELLED_DIALOG_TEXT);
	note->SetTextL(dumpCancelledText->Des());
	CleanupStack::PopAndDestroy(dumpCancelledText);

	note->ExecuteLD();

	iDumbThreadProgressChecker->Cancel();
}

void CDumberdoreAppUi::PullOffErrorFromProgressUpdateL(const TDesC &aErr) {
	CAknErrorNote *note = new (ELeave) CAknErrorNote();
		
	note->SetTextL(aErr);
	note->ExecuteLD();

	iProgressDialog->ProcessFinishedL();
	iDumbThreadProgressChecker->Cancel();
}

TBool CDumberdoreAppUi::UpdateProgressBar() {
	if (!iSizeChecked && (iTargetSize != 0)) {
		RFs &theFs = iCoeEnv->FsSession();
		
		TInt driveNumber = EDriveE;
		theFs.CharToDrive(iDumbThreadData.iPath[0], driveNumber);
		
		TVolumeInfo driveVolume;
		theFs.Volume(driveVolume, driveNumber);
		
		// Dont want full either
		if (driveVolume.iFree <= iTargetSize) {
			HBufC* notEnoughSpaceDiag = iEikonEnv->AllocReadResourceLC(R_DUMP_NOT_ENOUGH_SPACE_DIALOG_TEXT);
			
			RBuf noteErrorBuf;
			noteErrorBuf.CreateL(notEnoughSpaceDiag->Length() + 10);
			noteErrorBuf.CleanupClosePushL();
			noteErrorBuf.Format(notEnoughSpaceDiag->Des(), iTargetSize / 1024 / 1024);
			
			PullOffErrorFromProgressUpdateL(noteErrorBuf);
			
			CleanupStack::PopAndDestroy(2, notEnoughSpaceDiag);
			
			return ETrue;
		}

		iSizeChecked = ETrue;
	}
	
	if (iDumbThread.Handle() == 0) {
		iDumbThreadData.iProgress = 0;
		iDumbThreadData.iShouldStop = EFalse;

		HBufC* titleMsg = iEikonEnv->AllocReadResourceLC((iTargetSize == 0) ? R_DUMP_ESTIMATE_DIALOG_TEXT : R_DUMP_IN_PROCESS_DIALOG_TEXT);
		iProgressDialog->SetTextL(titleMsg->Des());
		iDumbThreadData.iMode = (iTargetSize == 0) ? 1 : 0;
		
		CleanupStack::PopAndDestroy(titleMsg);
		
		TInt threadCreateResult = iDumbThread.Create(_L("RPKG Dump thread"), DumpRPKGThreadFunc, 0x5000, NULL, &iDumbThreadData, EOwnerProcess);
		
		if (threadCreateResult != KErrNone) {
			HBufC* errorCreateThreadText = iEikonEnv->AllocReadResourceLC(R_DUMP_CREATE_THREAD_ERROR_DIALOG_TEXT);
			
			RBuf errorMsg;
			errorMsg.CreateL(errorCreateThreadText->Length() + 10);
			errorMsg.CleanupClosePushL();
			errorMsg.Format(errorCreateThreadText->Des(), threadCreateResult);
			
			PullOffErrorFromProgressUpdateL(errorMsg);
			
			CleanupStack::PopAndDestroy(2, errorCreateThreadText);
			
			return EFalse;
		}
		
		iDumbThread.Resume();
	}
	
	if (iDumbThread.ExitType() != EExitPending) {
		RDebug::Printf("Finished: %d %d", (int)iDumbThread.ExitType(), iDumbThread.ExitReason());
		
		TBool shouldReturn = EFalse;

		if (iDumbThread.ExitReason() != KErrNone) {
			HBufC* dumpFailureMsg = iEikonEnv->AllocReadResourceLC(R_DUMP_ERROR_ENCOUTERED_DIALOG_TEXT);
		
			TExitCategoryName errCate = iDumbThread.ExitCategory();
			RBuf errMsg;
			errMsg.CreateL(dumpFailureMsg->Length() + 10 + errCate.Length());
			errMsg.CleanupClosePushL();
			errMsg.Format(dumpFailureMsg->Des(), iDumbThread.ExitReason(), &errCate);
			
			PullOffErrorFromProgressUpdateL(errMsg);
			
			CleanupStack::PopAndDestroy(2, dumpFailureMsg);
			
			shouldReturn = ETrue;
		} else if (iTargetSize == 0) {
			iTargetSize = iDumbThreadData.iProgress;
			iDumbThreadData.iProgress = 0;
		} else {
			CAknInformationNote *note = new (ELeave) CAknInformationNote();
			
			HBufC* dumpSuccessMsg = iEikonEnv->AllocReadResourceLC(R_DUMP_DONE_DIALOG_TEXT);
			note->SetTextL(dumpSuccessMsg->Des());
			CleanupStack::PopAndDestroy();
			
			note->ExecuteLD();
			
			iProgressDialog->ProcessFinishedL();
			iDumbThreadProgressChecker->Cancel();
			
			// We are done, dumping then else? Nothing
			shouldReturn = ETrue;
		}

		iDumbThread.Close();
		iDumbThread.SetHandle(0);
		
		if (shouldReturn)
			return ETrue;
	}
	
	if (iTargetSize == 0) {
		return ETrue;
	}

	CEikProgressInfo *info = iProgressDialog->GetProgressInfoL();
	info->SetAndDraw(static_cast<TInt>(iDumbThreadData.iProgress * 100 / iTargetSize));

	return ETrue;
}

void CDumberdoreAppUi::HandleDumpRPKG() {
	// Check is it possible to read data from system folders
	if (!CheckAccessToSystemFiles())
		{
		TRAP_IGNORE(ShowErrorL(R_DUMP_ACCESS_DENIED_DIALOG_TEXT));
		return;
		}
	
	// Let user browse the file first
	_LIT(KDumpRPKGFolderDriveChooser, "Choose destination drive");
	_LIT(KDumpRPKGFolderBrowserTitle, "Choose destination folder");

	CAknMemorySelectionDialog *driveDialog = NULL;
	TRAPD(err, driveDialog = CAknMemorySelectionDialog::NewL(ECFDDialogTypeBrowse, EFalse));
	
	if (err != KErrNone) {
		return;
	}
	
	driveDialog->SetTitleL(KDumpRPKGFolderDriveChooser);
	driveDialog->SetObserver(NULL);
	
	CAknMemorySelectionDialog::TMemory selectedMemory = CAknMemorySelectionDialog::EPhoneMemory;
	TBool selectResult = driveDialog->ExecuteL(selectedMemory);

	delete driveDialog;

	if (!selectResult) {
		// We have to stop here.
		return;
	}
	
	CAknFileSelectionDialog *fileDialog = NULL;
	TRAP(err, fileDialog = CAknFileSelectionDialog::NewL(ECFDDialogTypeBrowse));
	
	if (err != KErrNone) {
		return;
	}

	iDumbThreadData.iPath.SetLength(0);
	iDumbThreadData.iPath.Append((selectedMemory == CAknMemorySelectionDialog::EPhoneMemory) ? PathInfo::PhoneMemoryRootPath()
		: PathInfo::MemoryCardRootPath());
	
	fileDialog->SetDefaultFolderL(iDumbThreadData.iPath);
	fileDialog->SetTitleL(KDumpRPKGFolderBrowserTitle);
	
	selectResult = fileDialog->ExecuteL(iDumbThreadData.iPath);
	
	delete fileDialog;
	
	if (!selectResult) {
		return;
	}
	
	// We got the folder now. Now we want to do the dumping.
	iDumbThreadData.iProgress = 0;
	iDumbThreadData.iPath.Append(_L("SYM.RPKG"));
	iDumbThread.SetHandle(0);
	iSizeChecked = EFalse;
	
	iProgressDialog = new (ELeave) CAknProgressDialog((REINTERPRET_CAST(CEikDialog**,&iProgressDialog)));
	iProgressDialog->PrepareLC(R_PROGRESS_NOTE);
	CEikProgressInfo *progressDialogInfo = iProgressDialog->GetProgressInfoL();
	
	progressDialogInfo->SetFinalValue(100);

	iProgressDialog->SetCallback(this);
	iProgressDialog->RunLD();

    TCallBack updateCallback;
	updateCallback.iFunction = DumpRPKGProgressBarUpdater;
    updateCallback.iPtr = this;
    
	iDumbThreadProgressChecker->Start(0, 150, updateCallback);
}

void CDumberdoreAppUi::HandleAbout() {
	CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
	dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
	HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
	dlg->QueryHeading()->SetTextL(*title);
	CleanupStack::PopAndDestroy(); //title
	HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
	dlg->SetMessageTextL(*msg);
	CleanupStack::PopAndDestroy(); //msg
	dlg->RunLD();
}

// -----------------------------------------------------------------------------
// CDumberdoreAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CDumberdoreAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			Exit();
			break;

		case EDumpRPKG:
			HandleDumpRPKG();
			break;

		case EAbout:
			HandleAbout();
			break;

		default:
			Panic( EDumberdoreUi);
			break;
		}
	}
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CDumberdoreAppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect(ClientRect());
	}

CArrayFix<TCoeHelpContext>* CDumberdoreAppUi::HelpContextL() const
	{
	return NULL;
	}

TBool CDumberdoreAppUi::CheckAccessToSystemFiles()
	{
	_LIT(KPath, "z:\\sys\\");
	CDir* files = NULL;
	TInt r = iCoeEnv->FsSession().GetDir(KPath, KEntryAttMatchMask, ESortNone, files);
	TInt ret = r == KErrNone && files != NULL;
	delete files;
	return ret;
	}

void CDumberdoreAppUi::ShowErrorL(const TDesC &aMsg)
	{
	CAknErrorNote* errNote = new (ELeave) CAknErrorNote;
	CleanupStack::PushL(errNote);
	errNote->SetTextL(aMsg);
	CleanupStack::Pop(errNote);
	errNote->ExecuteLD();
	}

void CDumberdoreAppUi::ShowErrorL(TInt aResourceId)
	{
	HBufC* msg = iEikonEnv->AllocReadResourceLC(aResourceId);
	ShowErrorL(msg->Des());
	CleanupStack::PopAndDestroy(msg);
	}

// End of File
