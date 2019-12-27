#include <Dumber/DumberBootstrap.h>
#include <Dumber/DumberBuilder.h>
#include <Dumber/DumberConsole.h>
#include <Dumber/DumberDumb.h>
#include <Dumber/DumberFile.h>

using namespace Dumber;

void MainL() {
  _LIT(KRomName, "E:\\SYM.ROM");
  _LIT(KSuccMsg, "Success dumping the ROM. Press key to exit.\n");
  _LIT(KFailMsg, "Romdump failure. Press key to exit.\n");

  TPtrC RomName(KRomName);
  TPtrC SuccMsg(KSuccMsg);
  TPtrC FailMsg(KFailMsg);

  Dumber::TDumberConsole* cons = Dumber::GetConsole();

  TDumberBuilder builder;
  builder.ConstructL();

  TPtrC romName(_L("E:\\SYM.RPKG"));

  TRAPD(dumbRes, builder.BuildRpkgL(romName));

  if (dumbRes != KErrNone) {
    cons->Print(FailMsg);
    cons->Getch();
  } else {
    cons->Print(SuccMsg);
    cons->Getch();
  }
}

TInt E32Main() {
  __UHEAP_MARK;
  Bootstrap();

  TRAPD(err, InitConsoleAppL());

  if (err) {
    return err;
  }

  TRAP(err, MainL());

  FreeApp();

  __UHEAP_MARKEND;
  return err;
}
