#include "maditorsharedlib.h"


#include "AppControl.h"

#include "Network\networkmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

		AppControl::AppControl() :
			mNetwork(new Engine::Network::NetworkManager),
			mMemory(new SharedMemory)
		{			
			mNetwork->addTopLevelItem(this);
		}

		AppControl::AppControl(size_t id) :
			mNetwork(new Engine::Network::NetworkManager),
			mMemory(new SharedMemory(id))
		{
			mNetwork->addTopLevelItem(this);
		}

AppControl::~AppControl()
{
	delete mMemory;
	delete mNetwork;
}

size_t AppControl::appId()
{
	return mMemory->id();
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
