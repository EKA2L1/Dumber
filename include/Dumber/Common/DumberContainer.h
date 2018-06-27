#ifndef DUMBERCONTAINER_H_
#define DUMBERCONTAINER_H_

#include <Dumber/DumberErr.h>

#include <e32cmn.h>
#include <e32base.h>
#include <e32std.h>
#include <e32debug.h>

_LIT(KDumberCagetoryLit, "DUMBER");

namespace Dumber {
    template <typename T>
    class TContainer {
    	T     *iData;
    	TUint  iSize;
    	TUint  iCap;
    	
    protected:
    	
    	/* \brief Reserve the memory with new capacity.
    	 * 
    	 * Leave: KErrNoMemory if there's no memory available.
    	 */
    	void ReserveL(TUint iNewCap) {
    	    if (iData == NULL) {
    	    	iData = reinterpret_cast<T*>(User::AllocZL(iNewCap * sizeof(T)));
    	    	
    	    	if (iData == NULL) {
    	    		User::Leave(KErrNoMemory);
    	    	}
    	    } else {
    	    	iData = reinterpret_cast<T*>(User::ReAllocL(reinterpret_cast<TAny*>(iData),
    	    			iNewCap * sizeof(T), 
    	    			2));
    	    	
    	    	if (iData == NULL) {
    	    		User::Leave(KErrNoMemory);
    	    	}
    	    }
    	    
    	    iCap = iNewCap;
    	    
    	    if (iCap < iSize) {
    	    	iSize = iCap;
    	    }
    	}
    	
    public:
    	TContainer() 
    	    : iData(0), iSize(0), iCap(0) {
    	}
    	
    	~TContainer() {
    		if (iData != NULL) {
    			User::Free(iData);
    		}
    	}
    	
    	void ConstructL(TUint aInitCap) {
    	    ReserveL(aInitCap);	
    	} 
    	
    	void PushL(const T aData) {
    		if (iSize == iCap) {
    	    	ReserveL((iCap + 1) * 2);
    	    }
    	    
    	    iData[++iSize - 1] = aData;
    	}
    	
    	void Pop() {
    		if (iSize > 0) {
    			iSize -= 1;
    		}
    	}
    	
    	void ResizeL(TUint aNewSize) {
    	    if (aNewSize > iCap) {
    	    	ReserveL(aNewSize);
    	    }

	    	iSize = aNewSize;
    	}
    	
    	T &At(TUint aIdx) {
    		if (aIdx >= iSize) {
				TPtrC DumberCage(KDumberCagetoryLit);
				User::Panic(DumberCage, KDumberOutOfRange);	
			}
			
			return iData[aIdx];
    	}
    	
    	T &operator [](TUint aIdx) {
    		return At(aIdx);
    	}
    	
    	T *Ptr() const {
    		return iData;
    	}
    	
    	TUint Size() const {
    		return iSize;
    	}
    	
    	TUint Capacity() const {
    		return iCap;
    	}
    	
    	T& Back() {
    		return iData[iSize - 1];
    	}
    	
    	void RemoveL(TUint aIdx) {
    		if (aIdx == iSize - 1) {
    			Pop();
    		}
    		
    		Mem::Move(Ptr() + aIdx, Ptr() + aIdx + 1, (iSize - 1) * sizeof(T));
    	}
    };
}

#endif
