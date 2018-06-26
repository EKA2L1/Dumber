#ifndef DUMBERDUMB_H_
#define DUMBERDUMB_H_

#include <e32cmn.h>

namespace Dumber {
    TInt Dumb(TDesC &aPath, TUint32 aAddress, TUint32 aSize);
    TInt DumbROM(TDesC &aPath);
}

#endif
