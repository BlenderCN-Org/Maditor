#include "maditorserverlauncherlib.h"

#include "ServerWrapper.h"

int main() {
	Maditor::Launcher::ServerWrapper server;
	return server.init();
}