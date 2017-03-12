#include "maditorsharedlib.h"


#include "AppControl.h"

#include "Network\networkmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

AppControlBase::AppControlBase(bool isLauncher) :
	TopLevelSerializableUnitBase(Engine::Serialize::RESERVED_ID_COUNT-1),
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

AppControlBase::~AppControlBase()
{
	delete mMemory;
	delete mNetwork;
}

Shared & AppControlBase::sharedMemory()
{
	return mMemory->data();
}

Engine::Network::NetworkManager * AppControlBase::network()
{
	return mNetwork;
}

void AppControlBase::resizeWindowImpl()
{
}

void AppControlBase::pingImpl()
{
}

	}
}
