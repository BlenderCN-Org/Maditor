#include "maditorsharedlib.h"


#include "AppControl.h"

#include "Network\networkmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

AppControl::AppControl(bool isLauncher) :
	TopLevelSerializableUnit(Engine::Serialize::MADITOR),
	mNetwork(new Engine::Network::NetworkManager),
	shutdown(this, &AppControl::shutdownImpl),
	start(this, &AppControl::startImpl),
	stop(this, &AppControl::stopImpl),
	pause(this, &AppControl::pauseImpl),
	applicationInitialized(this, &AppControl::onApplicationInitialized)
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
