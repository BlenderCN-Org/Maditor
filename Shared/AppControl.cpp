#include "maditorsharedlib.h"


#include "AppControl.h"

#include "Network\networkmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

Maditor::Shared::AppControl::AppControl(bool isLauncher) :
	TopLevelSerializableUnit(Engine::Serialize::MADITOR),
	mNetwork(new Engine::Network::NetworkManager),
	shutdown(this, &AppControl::shutdownImpl)
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

	}
}
