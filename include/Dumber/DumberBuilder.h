#ifndef DUMBERFS_H_
#define DUMBERFS_H_

#include <Dumber/Common/DumberContainer.h>
#include <Dumber/DumberFile.h>
#include <Dumber/DumberConsole.h>

#include <e32rom.h>
#include <f32file.h>

namespace Dumber {
    /*! \brief Class to build ROM 
     * 
     * */

    struct TDumberEntry {
    	TEntry iEntry;
    	TBool  iProcessed;
    	TUint  iDataPos;
    	TUint  iEntryPos;
    	
    	TUint  iSize;
    };

    struct TDumberDir {
    	TBufC<256> iName;
    	TUint iAtt;
    	
    	TContainer<TDumberEntry> iFiles;
    	TContainer<TDumberDir> iDirs;
    	
    	TBool iProcessed;
    	
    	TUint iEntryDataPtrPos;
    	TUint iEntryPos;
    	
    	TUint iDirPos;
    	TUint iEntrySize;
    	
    	TUint iDirBinSize;
    };

    class TDumberBuilder {
    	TDumberDir  iRoot;
    	TDumberFile* iFile;
    	
    	TRomHeader iHeader;
    	
    	TDumberConsole *iCons;
    	RFs *iFs;
    	
    	TUint iMaxReachedPos;
    	
    protected:
    	void CopyFileL(TDumberFile *aFile);
    	
    	TUint AdjustAddr(TUint aAddr);
    	
      	void BuildRomTreeL();
      	void BuildDirL(TDumberDir &aDir);
      	
      	void BuildRomHeaderL();
      	void BuildXIPDllsL();
      	
      	void BuildNormalFilesL(TDumberDir &aDir);
      	void BuildNormalFilesL();
      	
      	void BuildXIPDllL(TDumberFile *aFile, TUint &aWritePos);
      	void BuildNormalFileL(TDumberFile *aFile, TUint &aWritePos);
      	
      	void WriteFileEntry(TDumberEntry &aEntry);
      	void WriteDirEntry(TDumberDir &dir);
      	
      	void WriteDir(TDumberDir &dir);
      	
      	void WriteL(TDumberDir &dir);
      	void WriteL();
      	
      	/*! \brief Check if the file is XIP.
      	 */
      	TBool IsXIPL(TDumberFile &aFile);
      	TDumberFile *GetFileL(TEntry &aEntry);
      	
    public:
      	TDumberBuilder();
      	~TDumberBuilder();
      	
      	void ConstructL();
      	
      	void BuildRomL(TDesC &aName);
    };
}

#endif
