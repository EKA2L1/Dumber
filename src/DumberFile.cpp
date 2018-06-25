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

    		case EDumberReplaceRead: {
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
    	}
    	
    	User::Leave(KErrInvalidRequest);
    }

    void TDumberFile::ReadL(TDes8 &aData) {
    	if (iOpenMode == EDumberReplaceRead || iOpenMode == EDumberOpenRead) {
			User::LeaveIfError(iFile.Read(aData));
		}
		
		User::Leave(KErrInvalidRequest);	
    }

    void TDumberFile::Seek(const TUint64 aOff, const TDumberSeekMode aMode) {
    	switch (aMode) {
    		case EDumberSeekSet: {
    		    iFile.Seek(ESeekStart, aOff);
    		    break;
    		}
    		
    		case EDumberSeekCur: {
    		     iFile.Seek(ESeekCurrent, aOff);
    		     break;
    		}
    		
    		case EDumberSeekEnd: {
    		     iFile.Seek(ESeekEnd, aOff);
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
}
