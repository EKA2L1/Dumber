#ifndef DUMBERBOOTSTRAP_H_
#define DUMBERBOOTSTRAP_H_

class CTrapCleanup;

namespace Dumber {
    TInt Bootstrap();
    void FreeApp();
    
    CTrapCleanup *GetTrapCleanup();
}

#endif
