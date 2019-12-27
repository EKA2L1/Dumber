#ifndef DUMBERFILE_H_
#define DUMBERFILE_H_

#include <e32cmn.h>
#include <e32std.h>

#include <f32file.h>

namespace Dumber {
enum TDumberOpenMode {
  EDumberOpen = 0x235495,
  EDumberReplace = 0x235140,
  EDumberRead = 0x432041,
  EDumberWrite = 0x512987,
  EDumberOpenRead = EDumberOpen | EDumberRead,
  EDumberOpenWrite = EDumberOpen | EDumberWrite,
  EDumberReplaceRead = EDumberReplace | EDumberRead,
  EDumberReplaceWrite = EDumberReplace | EDumberWrite
};

enum TDumberSeekMode { EDumberSeekSet, EDumberSeekCur, EDumberSeekEnd };

class TDumberFile {
  TDumberOpenMode iOpenMode;
  RFile iFile;
  RFs* iFs;

 public:
  TDumberFile();
  ~TDumberFile();

  TUint64 Tell();

  void ConstructL(TDesC& aFileName, const TDumberOpenMode aMode);
  void WriteL(TDesC8& aData);
  void WriteL(TDesC16& aData);
  void ReadL(TDes8& aData);

  static TDumberFile* NewL(TDesC& aFileName, TDumberOpenMode aMode);
  static TDumberFile* NewLC(TDesC& aFileName, TDumberOpenMode aMode);

  void Seek(const TUint64 aOff, const TDumberSeekMode aMode);
  void WriteL(const TUint64 aOff, TDesC8& aContent);

  TUint64 Size() const;
};
}  // namespace Dumber

#endif /* DUMPERFILE_H_ */
