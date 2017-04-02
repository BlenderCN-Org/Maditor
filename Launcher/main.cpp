#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

int main() {
	Maditor::Launcher::ApplicationWrapper app;
	return app.start();
}