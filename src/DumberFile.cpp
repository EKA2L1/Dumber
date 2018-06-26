#include <Dumber/DumberFile.h>
#include <Dumber/DumberErr.h>

namespace Dumber {
    TDumberFile::TDumberFile() {
    	
    }
    
    TDumberFile::~TDumberFile() {
    	iFile.Close();
    	iFs.Close();
    }

    void TDumberFile::ConstructL(TDesC &aFileName, const TDumberOpenMode aMode) {
    	iOpenMode = aMode;
   
    	User::LeaveIfError(iFs.Connect(12));
    	
    	switch (iOpenMode) {
    		case EDumberOpenRead: {
    		    User::LeaveIfError(iFile.Open(iFs, aFileName, EFileRead));
    		    break;
    		}
    		
    		case EDumberOpenWrite: {
    		    User::LeaveIfError(iFile.Open(iFs, aFileName, EFileWrite));
    		    break;
    		}
    		
    		case EDumberReplaceRead: {
    		    User::LeaveIfError(iFile.Replace(iFs, aFileName, EFileRead));
    		    break;
    		}

    		case EDumberReplaceWrite: {
    		    User::LeaveIfError(iFile.Replace(iFs, aFileName, EFileWrite));
    		    break;
    		}
    		
    		default: {
    			User::Leave(KErrUnknownMode);
    			break;
    		}
    	}
    }
    
    void TDumberFile::WriteL(TDesC8 &aData) {
    	if (iOpenMode == EDumberReplaceWrite || iOpenMode == EDumberOpenWrite) {
    		User::LeaveIfError(iFile.Write(aData));
    		return;
    	}
    	
    	User::Leave(KErrInvalidRequest);
    }

    void TDumberFile::ReadL(TDes8 &aData) {
    	if (iOpenMode == EDumberReplaceRead || iOpenMode == EDumberOpenRead) {
			User::LeaveIfError(iFile.Read(aData));
			return;
		}
		
		User::Leave(KErrInvalidRequest);	
    }

    void TDumberFile::Seek(const TUint64 aOffset, const TDumberSeekMode aMode) {
    	TInt off = static_cast<TInt>(aOffset);
    	
    	switch (aMode) {
    		case EDumberSeekSet: {
    		    iFile.Seek(ESeekStart, off);
    		    break;
    		}
    		
    		case EDumberSeekCur: {
    		     iFile.Seek(ESeekCurrent, off);
    		     break;
    		}
    		
    		case EDumberSeekEnd: {
    		     iFile.Seek(ESeekEnd, off);
    		     break;
    		}
    		
    		default:
    			break;
    	}
    }
    
    TUint64 TDumberFile::Size() const {
    	TInt lSize = 0;
    	TInt lErr = iFile.Size(lSize);
    	
    	if (lErr == KErrNone) {
    		return static_cast<TUint64>(lSize);
    	}
    	
    	return 0;
    }

    TDumberFile *TDumberFile::NewL(TDesC &aFileName, TDumberOpenMode aMode) {
    	TDumberFile *file = new (ELeave) TDumberFile;
    	CleanupStack::PushL(file);
    	file->ConstructL(aFileName, aMode);
    	CleanupStack::Pop(file);
    	
    	return file;
    }
    
    TDumberFile *TDumberFile::NewLC(TDesC &aFileName, TDumberOpenMode aMode) {
    	TDumberFile *file = new (ELeave) TDumberFile;
		CleanupStack::PushL(file);
		file->ConstructL(aFileName, aMode);
		return file;
    }
}
