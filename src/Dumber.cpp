#include <Dumber/DumberFile.h>
#include <Dumber/DumberBootstrap.h>

TInt E32Main() {
	Dumber::Bootstrap();
	
	Dumber::FreeApp();
	
	return 0;
}
