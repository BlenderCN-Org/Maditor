#include "maditorsharedlib.h"


#include "AppControl.h"

#include "Network\networkmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

AppControl::AppControl(bool isLauncher) :
	TopLevelSerializableUnitBase(Engine::Serialize::RESERVED_ID_COUNT-1),
	mNetwork(new Engine::Network::NetworkManager)
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
