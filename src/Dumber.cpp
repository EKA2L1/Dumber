#include <Dumber/DumberFile.h>
#include <Dumber/DumberBootstrap.h>
#include <Dumber/DumberConsole.h>
#include <Dumber/DumberDumb.h>

using namespace Dumber;

void MainL() {
	TDumberConsole *cons = TDumberConsole::NewLC();
	
	_LIT(KRomName, "E:\\SYM.ROM");
	_LIT(KSuccMsg, "Success dumping the ROM");
	_LIT(KFailMsg, "Romdump failure.");
	
	TPtrC RomName(KRomName);
	TPtrC SuccMsg(KSuccMsg);
	TPtrC FailMsg(KFailMsg);
	
	TInt dumbRes = Dumber::DumbROM(RomName);
	
	if (dumbRes != KErrNone) {
		cons->Print(FailMsg);
		cons->Getch();
	} else {
	   cons->Print(SuccMsg);
	   cons->Getch();
	}
	
	CleanupStack::PopAndDestroy(cons);
}

TInt E32Main() {
	__UHEAP_MARK;
	TInt res = Bootstrap();
	
	if (res != KErrNone) {
		return res;
	}
	
	TRAPD(err, MainL());
	
	FreeApp();
	
	__UHEAP_MARKEND;
	return err;
}
