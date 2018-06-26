#ifndef DUMBERCONS_H_
#define DUMBERCONS_H_

#include <e32std.h>
#include <e32cmn.h>
#include <e32cons.h>

namespace Dumber {
    class TDumberConsole {
    	CConsoleBase *iConsole;
    	
    public:
    	TDumberConsole();
    	~TDumberConsole();
    	
    	void ConstructL();
    	
    	static TDumberConsole *NewL();
    	static TDumberConsole *NewLC();
    	
    	void Print(TDesC16 &aFormat);
    	void Getch();
    	
    	CConsoleBase *GetRawHandler() const;
    };
}

#endif
