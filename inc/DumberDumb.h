#ifndef DUMBERDUMB_H_
#define DUMBERDUMB_H_

#include <e32cmn.h>

namespace Dumber {
TInt Dumb(TDesC& aPath, TUint32 aAddress, TUint32 aSize, TInt &aProgress);
TInt DumbROM(TDesC& aPath, TInt &aProgress);
TInt BuildRpkgL(const TDesC& aDumbName, TInt64 &aProgress, TBool &aShouldStop, TUint8 aEstimate);
}  // namespace Dumber

#endif
