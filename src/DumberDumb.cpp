#include <Dumber/DumberDumb.h>
#include <Dumber/DumberFile.h>

#include <e32debug.h>
#include <e32rom.h>

const TInt KWriteChunkSize = 0x10000;

namespace Dumber {
TInt Dumb(TDesC& aPath, TUint32 aAddress, TUint32 aSize) {
  TDumberFile* dumber = NULL;
  TRAPD(err, dumber = TDumberFile::NewL(aPath, EDumberReplaceWrite));

  if (err != KErrNone) {
    return err;
  }

  HBufC8* buf = NULL;

  TRAP(err, buf = HBufC8::NewL(KWriteChunkSize));

  if (err != KErrNone) {
    delete dumber;
    return err;
  }

  TPtr8 bufDes(buf->Des());

  TInt left = aSize;
  TInt chunkSize = 0;

  while (left) {
    chunkSize = left < KWriteChunkSize ? left : KWriteChunkSize;

    bufDes.Copy((const TUint8*)(aAddress), chunkSize);
    TRAP(err, dumber->WriteL(bufDes));

    if (err) {
      RDebug::Printf("Error!");

      delete dumber;
      delete buf;
      return err;
    }

    aAddress += chunkSize;
    left -= chunkSize;

    RDebug::Printf("Left: %d", left);
  }

  delete dumber;
  delete buf;
  return KErrNone;
}

TInt DumbROM(TDesC& aPath) {
  TUint addr = UserSvr::RomHeaderAddress();
  TRomHeader* aHeader = (TRomHeader*)(addr);

  TUint size = aHeader->iRomSize - 0xA1000;

  return Dumb(aPath, addr, size);
}
}  // namespace Dumber
