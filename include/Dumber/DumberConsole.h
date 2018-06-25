#ifndef DUMBERCONS_H_
#define DUMBERCONS_H_

#include <e32std.h>
#include <e32cmn.h>
#include <e32cons.h>
#include <e32debug.h>

namespace Dumber {
    class DumberConsole {
    	CConsoleBase *iConsole;
    	
    public:
    	DumberConsole();
    	~DumberConsole();
    	
    	void ConstructL();
    	void ConstructLC();
    	
    	void Print(TDesC16 &aFormat, ...);
    };
}

#endif
