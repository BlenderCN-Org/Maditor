#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\errorcodes.h"

int main(int argc, char *argv[]) {
	if (argc < 2) return Maditor::Shared::ILLEGAL_ARGUMENTS;
	size_t id;
	std::stringstream ss(argv[1]);
	ss >> id;
	if (id == 0)
		return Maditor::Shared::ILLEGAL_ARGUMENTS;
	Maditor::Launcher::ApplicationWrapper app(id);
	return app.start();
}