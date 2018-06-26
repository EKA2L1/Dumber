#ifndef DUMBERBOOTSTRAP_H_
#define DUMBERBOOTSTRAP_H_

#include <e32std.h>

struct CTrapCleanup;

namespace Dumber {
    TInt Bootstrap();
    void FreeApp();
    
    CTrapCleanup *GetTrapCleanup();
}

#endif
