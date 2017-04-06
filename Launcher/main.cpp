#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"


int main(int argc, char *argv[]) {
	if (argc < 2) return -1;
	size_t id;
	std::stringstream ss(argv[1]);
	ss >> id;
	if (id == 0)
		return -1;
	Maditor::Launcher::ApplicationWrapper app(id);
	return app.start();
}