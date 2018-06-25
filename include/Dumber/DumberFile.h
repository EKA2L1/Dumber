#ifndef DUMBERFILE_H_
#define DUMBERFILE_H_

#include <e32cmn.h>
#include <e32std.h>

#include <f32file.h>

namespace Dumber {
    enum TDumberOpenMode {
    	EDumberOpen,
    	EDumberReplace,
    	EDumberRead,
    	EDumberWrite,
    	EDumberOpenRead = EDumberOpen | EDumberRead,
    	EDumberOpenWrite = EDumberOpen | EDumberWrite,
    	EDumberReplaceRead = EDumberReplace | EDumberRead,
    	EDumberReplaceWrite = EDumberReplace | EDumberWrite
    };
    
    enum TDumberSeekMode {
    	EDumberSeekSet,
    	EDumberSeekCur,
    	EDumberSeekEnd
    };

    class TDumberFile {
        TDumberOpenMode iOpenMode;
        RFile iFile;
        RFs iFs;
        
    public:
        TDumberFile();
        ~TDumberFile();
        
        void ConstructL(TDesC &aFileName, const TDumberOpenMode aMode);
        void WriteL(TDesC8 &aData);
        void ReadL(TDes8 &aData);
        
        void Seek(const TUint64 aOff, const TDumberSeekMode aMode);
        
        TUint64 Size() const;
    };
}

#endif /* DUMPERFILE_H_ */
