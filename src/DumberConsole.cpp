#include <Dumber/DumberConsole.h>
#include <e32debug.h>

namespace Dumber {
TDumberConsole::TDumberConsole() : iConsole(0) {}

TDumberConsole::~TDumberConsole() {
  delete iConsole;
}

void TDumberConsole::ConstructL() {
  _LIT(KConsoleName, "DDumberConsole");
  iConsole = Console::NewL(KConsoleName, TSize(-1, -1));
}

TDumberConsole* TDumberConsole::NewL() {
  TDumberConsole* cons = TDumberConsole::NewLC();
  CleanupStack::Pop(cons);

  return cons;
}

TDumberConsole* TDumberConsole::NewLC() {
  TDumberConsole* cons = new (ELeave) TDumberConsole;
  CleanupStack::PushL(cons);

  cons->ConstructL();

  return cons;
}

void TDumberConsole::Print(TDesC16& aFormat) {
  iConsole->Write(aFormat);
  RDebug::Print(aFormat);
}

CConsoleBase* TDumberConsole::GetRawHandler() const {
  return iConsole;
}

void TDumberConsole::Getch() {
  iConsole->Getch();
}
}  // namespace Dumber
