#include <DumberErr.h>
#include <DumberFile.h>

namespace Dumber {
TDumberFile::TDumberFile(RFs &aFs)
  : iFs(&aFs)
  , iVirtualPos(0)
  , iMaxReached(0) {
}

TDumberFile::~TDumberFile() {
  iFile.Close();
}

void TDumberFile::ConstructL(const TDesC& aFileName, const TDumberOpenMode aMode) {
  iOpenMode = aMode;

  switch (iOpenMode) {
    case EDumberEstimate: {
	  iVirtualPos = 0;
	  iMaxReached = 0;
	  break;
    } 
  
    case EDumberOpenRead: {
      User::LeaveIfError(
          iFile.Open(*iFs, aFileName, EFileRead | EFileShareReadersOnly));
      break;
    }

    case EDumberOpenWrite: {
      User::LeaveIfError(
          iFile.Open(*iFs, aFileName, EFileWrite | EFileShareReadersOrWriters));
      break;
    }

    case EDumberReplaceRead: {
      User::LeaveIfError(
          iFile.Replace(*iFs, aFileName, EFileRead | EFileShareReadersOnly));
      break;
    }

    case EDumberReplaceWrite: {
      User::LeaveIfError(iFile.Replace(
          *iFs, aFileName, EFileWrite | EFileShareReadersOrWriters));
      break;
    }

    default: {
      User::Leave(KErrUnknownMode);
      break;
    }
  }
}

void TDumberFile::WriteL(TDesC8& aData) {
  if (iOpenMode == EDumberEstimate) {
    iVirtualPos += aData.Size();
    iMaxReached = iVirtualPos > iMaxReached ? iVirtualPos : iMaxReached;
    
    return;
  }

  if (iOpenMode == EDumberReplaceWrite || iOpenMode == EDumberOpenWrite) {
    User::LeaveIfError(iFile.Write(aData));
    return;
  }

  User::Leave(KErrInvalidRequest);
}

void TDumberFile::WriteL(TDesC16& aData) {
  if (iOpenMode == EDumberEstimate) {
    iVirtualPos += aData.Size();
    iMaxReached = iVirtualPos > iMaxReached ? iVirtualPos : iMaxReached;
    
    return;
  }

  TPtrC8 temp;
  temp.Set(reinterpret_cast<const TUint8*>(aData.Ptr()), aData.Length() * 2);

  if (iOpenMode == EDumberReplaceWrite || iOpenMode == EDumberOpenWrite) {
    User::LeaveIfError(iFile.Write(temp));
    return;
  }

  User::Leave(KErrInvalidRequest);
}

void TDumberFile::WriteL(const TUint64 aOff, TDesC8& aContent) {
  if (iOpenMode == EDumberEstimate) {
    iVirtualPos = aOff + aContent.Length();
    iMaxReached = iVirtualPos > iMaxReached ? iVirtualPos : iMaxReached;
    
    return;
  }
 
  if (iOpenMode == EDumberReplaceWrite || iOpenMode == EDumberOpenWrite) {
    User::LeaveIfError(iFile.Write(aOff, aContent));
    return;
  }

  User::Leave(KErrInvalidRequest);
}

void TDumberFile::ReadL(TDes8& aData) {
  if (iOpenMode == EDumberReplaceRead || iOpenMode == EDumberOpenRead) {
    User::LeaveIfError(iFile.Read(aData));
    return;
  }

  User::Leave(KErrInvalidRequest);
}

void TDumberFile::Seek(const TUint64 aOffset, const TDumberSeekMode aMode) {
  TInt off = static_cast<TInt>(aOffset);

  if (iOpenMode == EDumberEstimate) {
	switch (aMode) {
	  case EDumberSeekSet:
	    iVirtualPos = off;
	    break;
	    
	  case EDumberSeekCur:
		iVirtualPos += off;
		break;
		
	  case EDumberSeekEnd:
	  	iVirtualPos = iMaxReached + off;
	  	break;
	}
	
    if (iVirtualPos > iMaxReached) iMaxReached = off;
    return;
  }
  
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
  if (iOpenMode == EDumberEstimate)
    return iMaxReached;

  TInt lSize = 0;
  TInt lErr = iFile.Size(lSize);

  if (lErr == KErrNone) {
    return static_cast<TUint64>(lSize);
  }

  return 0;
}

TDumberFile* TDumberFile::NewL(RFs &aFs, const TDesC& aFileName, TDumberOpenMode aMode) {
  TDumberFile* file = new (ELeave) TDumberFile(aFs);
  CleanupStack::PushL(file);
  file->ConstructL(aFileName, aMode);
  CleanupStack::Pop(file);

  return file;
}

TDumberFile* TDumberFile::NewLC(RFs &aFs, const TDesC& aFileName, TDumberOpenMode aMode) {
  TDumberFile* file = new (ELeave) TDumberFile(aFs);
  CleanupStack::PushL(file);
  file->ConstructL(aFileName, aMode);
  return file;
}

TUint64 TDumberFile::Tell() {
  TInt temp = 0;
  iFile.Seek(ESeekCurrent, temp);

  return temp;
}
}  // namespace Dumber
