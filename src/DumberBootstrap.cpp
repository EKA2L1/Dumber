#include <Dumber/DumberBootstrap.h>

#include <e32base.h>
#include <e32cmn.h>
#include <e32std.h>

CTrapCleanup *gCleanup;
CActiveScheduler *gSche;

namespace Dumber {
    TInt Bootstrap() {
        gCleanup = CTrapCleanup::New();
        
        gSche = new (ELeave) CActiveScheduler();
        CActiveScheduler::Install(gSche);
        
        return KErrNone;
    }
    
    void FreeApp() {
    	delete gSche;
    	delete gCleanup;
    }

    CTrapCleanup *GetTrapCleanup() {
    	return gCleanup;
    }
}
