#include <Dumber/DumberBootstrap.h>
#include <Dumber/DumberBuilder.h>

#include <Dumber/DumberConsole.h>

namespace Dumber {
TDumberBuilder::TDumberBuilder() {
  iFs = Dumber::GetFs();
  iCons = Dumber::GetConsole();

  iFileCount = 0;
}

TDumberBuilder::~TDumberBuilder() {}

void TDumberBuilder::ConstructL() {}

TUint TDumberBuilder::AdjustAddr(TUint aAddr) {
  return aAddr - iHeader.iRomBase;
}

const TInt KWriteDumbChunkSize = 0x10000;

void TDumberBuilder::CopyFileL(TDumberFile* aFile) {
  HBufC8* buf = HBufC8::NewL(KWriteDumbChunkSize);

  TPtr8 bufDes(buf->Des());

  TInt left = aFile->Size();
  TInt chunkSize = 0;

  while (left) {
    chunkSize = left < KWriteDumbChunkSize ? left : KWriteDumbChunkSize;

    bufDes.SetLength(chunkSize);

    aFile->ReadL(bufDes);
    iFile->WriteL(bufDes);

    left -= chunkSize;
  }

  delete buf;
}

void TDumberBuilder::BuildDirL(TDumberDir& aDir) {
  CDir* files;
  CDir* dirs;

  TInt res =
      iFs->GetDir(aDir.iLongName, KEntryAttMatchMask, ESortByUid, files, dirs);

  User::LeaveIfError(res);

  RDebug::Print(_L("Parsing directory: %S, dir count: %d, file count: %d"),
                &aDir.iLongName, dirs->Count(), files->Count());

  aDir.iDirs.ResizeL(dirs->Count());
  aDir.iFiles.ResizeL(files->Count());

  for (TInt i = 0; i < dirs->Count(); i++) {
    TDumberDir& subdir = aDir.iDirs[i];

    RDebug::Printf("Attrib: %d", (*dirs)[i].iAtt);

    subdir.iAtt = (*dirs)[i].iAtt;
    subdir.iName = (*dirs)[i].iName;

    TPtrC namePtr = subdir.iName.Des();
    TPtr longNamePtr = subdir.iLongName.Des();

    longNamePtr.Append(aDir.iLongName);
    longNamePtr.Append(namePtr);
    longNamePtr.Append('\\');

    BuildDirL(subdir);
  }

  TUint aLen = 0;

  for (TInt i = 0; i < files->Count(); i++) {
    if ((*files)[i].iAtt & KEntryAttDir) {
      continue;
    }

    TDumberEntry& initEntry = aDir.iFiles[aLen];
    initEntry.iEntry = (*files)[i];

    TPtrC namePtr = initEntry.iEntry.iName.Des();
    TPtr longNamePtr = initEntry.iLongName.Des();

    iFs->RealName(namePtr, longNamePtr);

    aLen++;
  }
}

void TDumberBuilder::BuildRomTreeL() {
  _LIT(KRootName, "Z:\\");

  iRoot.iName = TPtrC(KRootName);
  iRoot.iLongName = iRoot.iName;

  iFs->SetSessionPath(iRoot.iName);

  BuildDirL(iRoot);
}

void TDumberBuilder::BuildRomHeaderL() {
  // This can be skipped and later to fill
  iHeader.iCompressionType = 0;
  iHeader.iCompressedSize = iFile->Size();
  iHeader.iRomBase = UserSvr::RomHeaderAddress();
  iHeader.iRomHeaderSize = sizeof(iHeader);

  TInt lastpos = iFile->Tell();
  iFile->Seek(0, EDumberSeekSet);

  TPckgC<TRomHeader> header(iHeader);
  iFile->WriteL(header);

  iFile->Seek(lastpos, EDumberSeekSet);
}

/*! \brief Set all XIP DLL in Z:\\sys\\bin to the dump file
 *
 */
void TDumberBuilder::BuildXIPDllsL() {
  /* Expect only 2 loops. */
  TDumberDir& curDir = iRoot;

  _LIT(KSysNameLit, "sys");
  TPtrC KSysName(KSysNameLit);

  _LIT(KBinNameLit, "bin");
  TPtrC KBinName(KBinNameLit);

  /* TODO: This can improve so much. */

  bool found = false;

  for (TUint32 j = 0; j < curDir.iDirs.Size(); j++) {
    if (curDir.iDirs[j].iName == KSysName) {
      curDir = curDir.iDirs[j];
      found = true;
      break;
    }
  }

  if (!found)
    User::Leave(KErrNotFound);

  found = false;

  for (TUint32 j = 0; j < curDir.iDirs.Size(); j++) {
    if (curDir.iDirs[j].iName == KBinName) {
      curDir = curDir.iDirs[j];
      break;
    }
  }

  if (!found)
    User::Leave(KErrNotFound);

  // The Dir should now be found
  for (TInt i = curDir.iFiles.Size() - 1; i >= 0; i--) {
    TDumberFile* dllFile =
        TDumberFile::NewL(curDir.iFiles[i].iLongName, EDumberOpenRead);

    if (IsXIPL(*dllFile)) {
      delete dllFile;
      continue;
    }

    BuildXIPDllL(dllFile, curDir.iFiles[i].iDataPos);

    curDir.iFiles[i].iProcessed = true;

    delete dllFile;
  }
}

void TDumberBuilder::BuildXIPDllL(TDumberFile* dllFile, TUint& aWritePos) {
  TRomImageHeader romImgHeader;
  TPckg<TRomImageHeader> romImgHeaderPkg(romImgHeader);

  dllFile->ReadL(romImgHeaderPkg);

  aWritePos = AdjustAddr(romImgHeader.iCodeAddress - sizeof(TRomImageHeader));

  TUint64 pos = iFile->Tell();
  iFile->Seek(aWritePos, EDumberSeekSet);
  CopyFileL(dllFile);
  iFile->Seek(pos, EDumberSeekSet);

  TUint after = (aWritePos + dllFile->Size());
  iMaxReachedPos = iMaxReachedPos < after ? after : iMaxReachedPos;
}

void TDumberBuilder::BuildNormalFileL(TDumberFile* aFile, TUint& aWritePos) {
  aWritePos = iFile->Tell();
  CopyFileL(aFile);
}

void TDumberBuilder::BuildNormalFilesL(TDumberDir& aDir) {
  for (TUint i = 0; i < aDir.iFiles.Size(); i++) {
    if (aDir.iFiles[i].iProcessed) {
      continue;
    }

    TPtr name = aDir.iFiles[i].iLongName.Des();
    TDumberFile* file = TDumberFile::NewL(name, EDumberOpenRead);

    BuildNormalFileL(file, aDir.iFiles[i].iDataPos);

    aDir.iFiles[i].iProcessed = true;

    delete file;
  }

  for (TUint i = 0; i < aDir.iDirs.Size(); i++) {
    // Build subdirs
    BuildNormalFilesL(aDir.iDirs[i]);
  }
}

void TDumberBuilder::BuildNormalFilesL() {
  BuildNormalFilesL(iRoot);
}

void TDumberBuilder::BuildRomL(TDesC& aName) {
  iFile = TDumberFile::NewL(aName, EDumberReplaceWrite);

  BuildRomTreeL();

  _LIT(KDumberBuildingRomLit, "Building placeholder ROM Header\n.");
  _LIT(KDumberBuildingXipDLLLit, "Building XIP DLLs\n.");

  TPtrC KDumberBuildingRom(KDumberBuildingRomLit);
  TPtrC KDumberBuildingXipDLL(KDumberBuildingXipDLLLit);
  TPtrC KDumberBuildingNormalFiles(_L("Buidling normal files.\n"));

  iCons->Print(KDumberBuildingRom);

  BuildRomHeaderL();
  iFile->Seek(iHeader.iRomHeaderSize, EDumberSeekSet);

  iCons->Print(KDumberBuildingXipDLL);
  BuildXIPDllsL();

  // Seek to the max reached pos that ROM image where written in
  iFile->Seek(iMaxReachedPos, EDumberSeekSet);
  iCons->Print(KDumberBuildingNormalFiles);
  BuildNormalFilesL();

  TPtrC KDumberWritingFileEntries(_L("Writing file entries.\n"));
  iCons->Print(KDumberWritingFileEntries);
  WriteL();

  BuildRomHeaderL();

  delete iFile;
}

TDumberFile* TDumberBuilder::GetFileL(TEntry& aEntry) {
  TBuf<256> longName;
  iFs->RealName(aEntry.iName, longName);
  return TDumberFile::NewLC(longName, EDumberOpenRead);
}

TBool TDumberBuilder::IsXIPL(TDumberFile& aFile) {
  TUint org = aFile.Tell();
  aFile.Seek(16, EDumberSeekSet);

  TInt sig;
  TPckg<TInt> sigp(sig);

  aFile.ReadL(sigp);
  aFile.Seek(org, EDumberSeekSet);

  if (sig == 0x434F5045) {
    return false;
  }

  return true;
}

void TDumberBuilder::WriteFileEntry(TDumberEntry& aEntry) {
  TRomEntry entry;
  entry.iAddressLin = aEntry.iEntryPos;
  entry.iAtt = aEntry.iEntry.iAtt;
  entry.iSize = aEntry.iEntry.iSize;
  entry.iNameLength = aEntry.iEntry.iName.Length();

  TPckgC<TRomEntry> entryPkg(entry);
  aEntry.iEntryPos = iFile->Tell();

  iFile->WriteL(entryPkg);
  iFile->Seek(aEntry.iEntryPos + 10, EDumberSeekSet);

  TPtr name = aEntry.iEntry.iName.Des();

  iFile->WriteL(name);

  aEntry.iSize = 10 + entry.iNameLength * 2;
}

void TDumberBuilder::WriteDirEntry(TDumberDir& aDir) {
  TRomEntry entry;
  entry.iAddressLin = 0;
  entry.iAtt = aDir.iAtt;
  entry.iSize = 0;
  entry.iNameLength = aDir.iName.Length();

  TPckgC<TRomEntry> entryPkg(entry);

  aDir.iEntryPos = iFile->Tell();
  aDir.iEntryDataPtrPos = aDir.iEntryPos + 4;
  aDir.iEntrySize = 10 + aDir.iName.Length() * 2;

  iFile->WriteL(entryPkg);
  iFile->Seek(aDir.iEntryPos + 10, EDumberSeekSet);

  TPtr name = aDir.iName.Des();

  iFile->WriteL(name);
}

void TDumberBuilder::WriteDir(TDumberDir& dir) {
  TUint iDirSize = 0;

  TPckgC<TInt> dirSize(iDirSize);
  dir.iDirPos = iFile->Tell();
  TUint dirSizePos = dir.iDirPos;

  for (TUint i = 0; i < dir.iFiles.Size(); i++) {
    WriteFileEntry(dir.iFiles[i]);
    iDirSize += dir.iFiles[i].iSize;
  }

  for (TUint i = 0; i < dir.iFiles.Size(); i++) {
    WriteDirEntry(dir.iDirs[i]);
    iDirSize += dir.iDirs[i].iEntrySize;
  }

  TPckgC<TInt> dirOff(dir.iDirPos);

  iFile->WriteL(dirSizePos, dirSize);
  iFile->WriteL(dir.iEntryDataPtrPos, dirOff);

  dir.iDirBinSize = iDirSize;
}

void TDumberBuilder::WriteL() {
  iHeader.iRomRootDirectoryList = iFile->Tell();

  TUint useless1 = 0;
  TUint addrLinear = iHeader.iRomRootDirectoryList + 8;

  TPckgC<TUint> uselessPkg(useless1);
  TPckgC<TUint> addrLinearPkg(addrLinear);

  iFile->WriteL(uselessPkg);
  iFile->WriteL(addrLinearPkg);

  WriteDir(iRoot);
}

struct TRpkgEntry {
  TUint64 iAtt;
  TTime iLastModified;
  TUint64 iFullPathLength;
};

struct TRpkgData {
  TUint64 iDataSize;
};

void TDumberBuilder::BuildRpkgL(CDir*& aDir, TDesC& aDirLongName) {
  for (TInt i = 0; i < aDir->Count(); i++) {
    const TEntry& ent = (*aDir)[i];

    HBufC16* LongName = HBufC16::NewL(256);
    TPtr aNextLongName = LongName->Des();
    aNextLongName.Append(aDirLongName);
    aNextLongName.Append(ent.iName);

    RDebug::Print(_L("Path: %S, att: %d"), &aNextLongName, ent.iAtt);

    if (ent.IsDir()) {
      CDir* subdir;
      aNextLongName.Append('\\');
      iFs->GetDir(aNextLongName, KEntryAttMatchMask, ESortByUid, subdir);

      BuildRpkgL(subdir, aNextLongName);
    } else {
      TDumberFile* file = nullptr;
      TRAPD(err, file = TDumberFile::NewL(aNextLongName, EDumberOpenRead));

      if (err != KErrNone) {
        RDebug::Print(_L("Skipping file: %s"), &LongName);
        continue;
      }

      iFileCount++;

      TRpkgEntry entryrpkg;
      entryrpkg.iFullPathLength = aNextLongName.Length();
      entryrpkg.iLastModified = ent.iModified;
      entryrpkg.iAtt = ent.iAtt;

      TPckgC<TRpkgEntry> entryrpkg_pkg(entryrpkg);
      iFile->WriteL(entryrpkg_pkg);
      iFile->WriteL(aNextLongName);  // Write name

      TRpkgData datarpkg;
      datarpkg.iDataSize = file->Size();

      TPckgC<TRpkgData> datarpkg_pkg(datarpkg);
      iFile->WriteL(datarpkg_pkg);

      CopyFileL(file);

      delete file;
    }

    delete LongName;
  }
}

struct TRpkgHeader {
  TChar iMagic[4];
  TVersion iAssociatedRomVersion;
  TUint iFileCount;
};

void TDumberBuilder::BuildRpkgL(TDesC& aDumbName) {
  iFile = TDumberFile::NewL(aDumbName, EDumberReplaceWrite);

  TRpkgHeader header;
  header.iMagic[0] = 'R';
  header.iMagic[1] = 'P';
  header.iMagic[2] = 'K';
  header.iMagic[3] = 'G';

  TPckgC<TRpkgHeader> headerpkg(header);

  TMachineInfoV1Buf infobuf;
  UserHal::MachineInfo(infobuf);

  TMachineInfoV1 machineInfo = infobuf();
  header.iAssociatedRomVersion = machineInfo.iRomVersion;

  iFile->WriteL(headerpkg);

  CDir* zDir;
  iFs->GetDir(_L("Z:\\"), KEntryAttMatchMask, ESortByUid, zDir);

  TPtrC zDirPath(_L("Z:\\"));
  BuildRpkgL(zDir, zDirPath);

  iFile->Seek(0, EDumberSeekSet);

  header.iFileCount = iFileCount;
  iFile->WriteL(headerpkg);
}
}  // namespace Dumber
