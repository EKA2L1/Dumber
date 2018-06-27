#ifndef DUMBERBOOTSTRAP_H_
#define DUMBERBOOTSTRAP_H_

#include <e32std.h>
#include <f32file.h>

struct CTrapCleanup;

namespace Dumber {
	class TDumberConsole;

	TInt Bootstrap();
	
	void InitConsoleAppL();
    void FreeApp();
    
    CTrapCleanup *GetTrapCleanup();
    RFs *GetFs();
    TDumberConsole *GetConsole();
}

#endif
