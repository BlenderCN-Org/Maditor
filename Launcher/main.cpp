#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\errorcodes.h"

#include "serialize/container/noparentunit.h"

int main(int argc, char *argv[]) {
	if (argc < 2) return Maditor::Shared::ILLEGAL_ARGUMENTS;
	size_t id;
	std::stringstream ss(argv[1]);
	ss >> id;
	if (id == 0)
		return Maditor::Shared::ILLEGAL_ARGUMENTS;
	Engine::Serialize::NoParentUnit<Maditor::Launcher::ApplicationWrapper> app(id);
	return app.start();
}