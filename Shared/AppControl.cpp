#include "maditorsharedlib.h"


#include "AppControl.h"

#include "Network\networkmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

AppControl::AppControl(bool isLauncher) :
	TopLevelSerializableUnit(Engine::Serialize::RESERVED_ID_COUNT-1),
	mNetwork(new Engine::Network::NetworkManager),
	shutdown(this),
	start(this),
	stop(this),
	pause(this),
	resizeWindow(this),
	ping(this),
	applicationInitialized(this)
{
	if (isLauncher) {
		mMemory = new SharedMemory(SharedMemory::create);
	}
	else {
		mMemory = new SharedMemory(SharedMemory::open);
	}
	mNetwork->addTopLevelItem(this);
}

AppControl::~AppControl()
{
	delete mMemory;
	delete mNetwork;
}

Shared & AppControl::sharedMemory()
{
	return mMemory->data();
}

Engine::Network::NetworkManager * AppControl::network()
{
	return mNetwork;
}

void AppControl::resizeWindowImpl()
{
}

void AppControl::pingImpl()
{
}

	}
}
