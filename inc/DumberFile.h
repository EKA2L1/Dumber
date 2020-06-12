#ifndef DUMBERFILE_H_
#define DUMBERFILE_H_

#include <e32cmn.h>
#include <e32std.h>

#include <f32file.h>

namespace Dumber {
enum TDumberOpenMode {
  EDumberOpen = 1 << 0,
  EDumberReplace = 1 << 1,
  EDumberRead = 1 << 2,
  EDumberWrite = 1 << 3,
  EDumberEstimate = 1 << 4,
  EDumberOpenRead = EDumberOpen | EDumberRead,
  EDumberOpenWrite = EDumberOpen | EDumberWrite,
  EDumberReplaceRead = EDumberReplace | EDumberRead,
  EDumberReplaceWrite = EDumberReplace | EDumberWrite
};

enum TDumberSeekMode { EDumberSeekSet, EDumberSeekCur, EDumberSeekEnd };

class TDumberFile {
  TDumberOpenMode iOpenMode;
  RFile iFile;
  RFs *iFs;

  TUint iVirtualPos;
  TUint iMaxReached;

public:
  TDumberFile(RFs &aFs);
  ~TDumberFile();

  TUint64 Tell();

  void ConstructL(const TDesC &aFileName, const TDumberOpenMode aMode);
  void WriteL(TDesC8 &aData);
  void WriteL(TDesC16 &aData);
  void ReadL(TDes8 &aData);

  static TDumberFile *NewL(RFs &aFs, const TDesC &aFileName,
                           TDumberOpenMode aMode);
  static TDumberFile *NewLC(RFs &aFs, const TDesC &aFileName,
                            TDumberOpenMode aMode);

  void Seek(const TUint64 aOff, const TDumberSeekMode aMode);
  void WriteL(const TUint64 aOff, TDesC8 &aContent);

  TUint64 Size() const;
};
} // namespace Dumber

#endif /* DUMPERFILE_H_ */
