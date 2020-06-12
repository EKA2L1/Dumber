#include <DumberDumb.h>
#include <DumberFile.h>

#include <e32debug.h>
#include <e32rom.h>

const TInt KWriteChunkSize = 0x10000;

namespace Dumber {
TInt Dumb(TDesC& aPath, TUint32 aAddress, TUint32 aSize, TInt &aProgress) {
  RFs fs;
  fs.Connect(-1);

  TDumberFile* dumber = NULL;
  TRAPD(err, dumber = TDumberFile::NewL(fs, aPath, EDumberReplaceWrite));

  if (err != KErrNone) {
	fs.Close();
    return err;
  }

  HBufC8* buf = NULL;

  TRAP(err, buf = HBufC8::NewL(KWriteChunkSize));

  if (err != KErrNone) {
    delete dumber;
    fs.Close();

    return err;
  }

  TPtr8 bufDes(buf->Des());

  TInt left = aSize;
  TInt chunkSize = 0;
  TInt originalSize = aSize;

  while (left) {
    chunkSize = left < KWriteChunkSize ? left : KWriteChunkSize;

    bufDes.Copy((const TUint8*)(aAddress), chunkSize);
    TRAP(err, dumber->WriteL(bufDes));

    if (err) {
      RDebug::Printf("Error!");
      fs.Close();
      
      delete dumber;
      delete buf;
      return err;
    }

    aAddress += chunkSize;
    left -= chunkSize;
    
    aProgress = 100 - (left * 100 / originalSize);
  }

  delete dumber;
  delete buf;
  
  fs.Close();
  return KErrNone;
}

TInt DumbROM(TDesC& aPath, TInt &aProgress) {
  TUint addr = UserSvr::RomHeaderAddress();
  TRomHeader* aHeader = (TRomHeader*)(addr);

  TUint size = aHeader->iRomSize - 0xA1000;

  return Dumb(aPath, addr, size, aProgress);
}

struct TRpkgHeader {
  TChar iMagic[4];
  TVersion iAssociatedRomVersion;
  TUint iFileCount;
  TUint iHeaderSize;
  TUint iMachineUID;
};

struct TRpkgEntry {
  TUint64 iAtt;
  TTime iLastModified;
  TUint64 iFullPathLength;
};

struct TRpkgData {
  TUint64 iDataSize;
};

const TInt KWriteDumbChunkSize = 0x10000;

static void CopyFileL(TDumberFile* aFile, TDumberFile *aDestFile) {
  HBufC8* buf = HBufC8::NewL(KWriteDumbChunkSize);

  TPtr8 bufDes(buf->Des());

  TInt left = aFile->Size();
  TInt chunkSize = 0;

  while (left) {
    chunkSize = left < KWriteDumbChunkSize ? left : KWriteDumbChunkSize;

    bufDes.SetLength(chunkSize);

    aFile->ReadL(bufDes);
    aDestFile->WriteL(bufDes);

    left -= chunkSize;
  }

  delete buf;
}

static void BuildRpkgL(CDir*& aDir, RFs *aFs, TDumberFile *aDestFile, TDesC& aDirLongName, TUint &aFileCount, TInt64 &aCopied, TBool &aShouldStop) {
  for (TInt i = 0; i < aDir->Count(); i++) {
	if (aShouldStop) {
		break;
	}

    const TEntry& ent = (*aDir)[i];

    HBufC16* LongName = HBufC16::NewL(1024);
    TPtr aNextLongName = LongName->Des();
    aNextLongName.Append(aDirLongName);
    aNextLongName.Append(ent.iName);

    //RDebug::Print(_L("Path: %S, att: %d"), &aNextLongName, ent.iAtt);

    if (ent.IsDir()) {
      CDir* subdir;
      aNextLongName.Append('\\');
      aFs->GetDir(aNextLongName, KEntryAttMatchMask, ESortByUid, subdir);

      BuildRpkgL(subdir, aFs, aDestFile, aNextLongName, aFileCount, aCopied, aShouldStop);
    } else {
      TDumberFile* file = NULL;
      TRAPD(err, file = TDumberFile::NewL(*aFs, aNextLongName, EDumberOpenRead));

      if (err != KErrNone) {
        RDebug::Print(_L("Skipping file: %s"), &LongName);
        continue;
      }

      aFileCount++;

      TRpkgEntry entryrpkg;
      entryrpkg.iFullPathLength = aNextLongName.Length();
      entryrpkg.iLastModified = ent.iModified;
      entryrpkg.iAtt = ent.iAtt;

      TPckgC<TRpkgEntry> entryrpkg_pkg(entryrpkg);
      aDestFile->WriteL(entryrpkg_pkg);
      aDestFile->WriteL(aNextLongName);  // Write name

      TRpkgData datarpkg;
      datarpkg.iDataSize = file->Size();

      TPckgC<TRpkgData> datarpkg_pkg(datarpkg);
      aDestFile->WriteL(datarpkg_pkg);

      CopyFileL(file, aDestFile);
      aCopied += file->Size();

      delete file;
    }

    delete LongName;
  }

  delete aDir;
}

TInt BuildRpkgL(const TDesC& aDumbName, TInt64 &aProgress, TBool &aShouldStop, TUint8 aEstimate) {
  RFs fs;
  fs.Connect(-1);

  TDumberFile *iFile = TDumberFile::NewL(fs, aDumbName, aEstimate ? EDumberEstimate : EDumberReplaceWrite);
  CleanupStack::PushL(iFile);

  TRpkgHeader header;
  header.iMagic[0] = 'R';
  header.iMagic[1] = 'P';
  header.iMagic[2] = 'K';
  header.iMagic[3] = '2';

  TPckgC<TRpkgHeader> headerpkg(header);

  TMachineInfoV1Buf infobuf;
  UserHal::MachineInfo(infobuf);

  TMachineInfoV1 machineInfo = infobuf();
  header.iAssociatedRomVersion = machineInfo.iRomVersion;
  header.iMachineUID = machineInfo.iMachineUniqueId;
  header.iHeaderSize = sizeof(TRpkgHeader);
  header.iFileCount = 0;

  iFile->WriteL(headerpkg);

  CDir* zDir;
  fs.GetDir(_L("Z:\\"), KEntryAttMatchMask, ESortByUid, zDir);
  
  TPtrC zDirPath(_L("Z:\\"));
  BuildRpkgL(zDir, &fs, iFile, zDirPath, header.iFileCount, aProgress, aShouldStop);

  iFile->Seek(0, EDumberSeekSet);
  iFile->WriteL(headerpkg);
  
  CleanupStack::PopAndDestroy();  // iFile
  
  return KErrNone;
}
}  // namespace Dumber
