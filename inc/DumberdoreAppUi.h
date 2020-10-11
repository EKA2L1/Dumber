/*
 ============================================================================
 Name		: DumberdoreAppUi.h
 Author	  : EKA2L1
 Copyright   : (C) 2020 EKA2L1 Project
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __DUMBERDOREAPPUI_h__
#define __DUMBERDOREAPPUI_h__

// INCLUDES
#include <aknappui.h>
#include <AknProgressDialog.h>

// FORWARD DECLARATIONS
class CDumberdoreAppView;

struct TDumberRPKGThreadData {
  TFileName iPath;
  TInt64 iProgress;
  TInt iMode;
  TBool iShouldStop;
};

// CLASS DECLARATION
/**
 * CDumberdoreAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CDumberdoreAppUi : public CAknAppUi, public MProgressDialogCallback
	{
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CDumberdoreAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CDumberdoreAppUi();

	/**
	 * ~CDumberdoreAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CDumberdoreAppUi();
	
	virtual void DialogDismissedL (TInt aButtonId);
	
	bool UpdateProgressBar();

private:
	void PullOffErrorFromProgressUpdate(const TDesC &aErr);
	
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
	CArrayFix<TCoeHelpContext>* HelpContextL() const;

private:
	
	/**
	 * @brief Handle the dump RPKG command. 
	 */
	void HandleDumpRPKG();
	
	/**
	 * @biref Handle the about command.
	 */
	void HandleAbout();
	
	TBool CheckAccessToSystemFiles();
	
	void ShowErrorL(const TDesC &aMsg);
	void ShowErrorL(TInt aResourceID);
	
	// Data

	/**
	 * The application view
	 * Owned by CDumberdoreAppUi
	 */
	CDumberdoreAppView* iAppView;
	
	CAknProgressDialog *iProgressDialog;
	
	RThread iDumbThread;
	
	TDumberRPKGThreadData iDumbThreadData;
	
	CPeriodic *iDumbThreadProgressChecker;
	
	TInt64 iTargetSize;
	
	bool iSizeChecked;
	};

#endif // __DUMBERDOREAPPUI_h__
// End of File
