#include <Dumber/DumberBootstrap.h>

#include <e32cmn.h>
#include <e32std.h>

CTrapCleanup *gCleanup;

namespace Dumber {
    TInt Bootstrap() {
        gCleanup = CTrapCleanup::New();
    }
    
    void FreeApp() {
    	delete gCleanup;
    }

    CTrapCleanup *GetTrapCleanup() {
    	return gCleanup;
    }
}
