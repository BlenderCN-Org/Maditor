#include "maditorsharedlib.h"


#include "AppControl.h"

#include "IPCManager\boostIPCmanager.h"

#include "SharedMemory.h"

namespace Maditor {
	namespace Shared {

		AppControl::AppControl() :
			mMemory(new SharedMemory),
			mNetwork(new BoostIPCManager(mMemory))
		{			
			mNetwork->addTopLevelItem(this);
		}

		AppControl::AppControl(size_t id) :
			mMemory(new SharedMemory(id)),
			mNetwork(new BoostIPCManager(mMemory))
		{
			mNetwork->addTopLevelItem(this);
		}

AppControl::~AppControl()
{
	delete mNetwork;
	delete mMemory;
}

size_t AppControl::appId()
{
	return mMemory->id();
}

SharedMemory * AppControl::mem()
{
	return mMemory;
}

Shared & AppControl::sharedMemory()
{
	return mMemory->data();
}

BoostIPCManager * AppControl::network()
{
	return mNetwork;
}

void AppControl::resizeWindowImpl()
{
}

void AppControl::pingImpl()
{
}

void AppControl::execLuaImpl(const std::string &)
{
}

	}
}
