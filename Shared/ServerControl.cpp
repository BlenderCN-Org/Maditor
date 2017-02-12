#include "maditorsharedlib.h"


#include "ServerControl.h"

#include "Network\networkmanager.h"

#include "ServerSharedMemory.h"

namespace Maditor {
	namespace Shared {

ServerControl::ServerControl(bool isLauncher) :
	TopLevelSerializableUnit(Engine::Serialize::RESERVED_ID_COUNT-1),
	mNetwork(new Engine::Network::NetworkManager),
	shutdown(this, &ServerControl::shutdownImpl),
	ping(this, &ServerControl::pingImpl)
{
	if (isLauncher) {
		mMemory = new ServerSharedMemory(ServerSharedMemory::create);
	}
	else {
		mMemory = new ServerSharedMemory(ServerSharedMemory::open);
	}
	mNetwork->addTopLevelItem(this);
}

ServerControl::~ServerControl()
{
	delete mMemory;
	delete mNetwork;
}

ServerInfo & ServerControl::sharedMemory()
{
	return mMemory->data();
}

Engine::Network::NetworkManager * ServerControl::network()
{
	return mNetwork;
}

void ServerControl::pingImpl()
{
}

	}
}
