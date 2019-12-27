#include <Dumber/DumberBootstrap.h>
#include <Dumber/DumberConsole.h>

#include <e32base.h>
#include <e32cmn.h>
#include <e32std.h>

Dumber::TDumberConsole* gConsole;

CTrapCleanup* gCleanup;
CActiveScheduler* gSche;
RFs gFs;

namespace Dumber {
TInt Bootstrap() {
  gCleanup = CTrapCleanup::New();
  return KErrNone;
}

void InitConsoleAppL() {
  gSche = new (ELeave) CActiveScheduler();
  CActiveScheduler::Install(gSche);
  gFs.Connect(12);

  gConsole = TDumberConsole::NewL();
}

void FreeApp() {
  gFs.Close();

  delete gSche;
  delete gConsole;
  delete gCleanup;
}

CTrapCleanup* GetTrapCleanup() {
  return gCleanup;
}

TDumberConsole* GetConsole() {
  return gConsole;
}

RFs* GetFs() {
  return &gFs;
}
}  // namespace Dumber
