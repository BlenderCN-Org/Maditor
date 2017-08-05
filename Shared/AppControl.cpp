#include "maditorsharedlib.h"


#include "AppControl.h"

#include "IPCManager\boostIPCmanager.h"

#include "SharedMemory.h"

#include "Serialize/toplevelids.h"

namespace Maditor {
	namespace Shared {

		AppControl::AppControl()
		{			
		}

		AppControl::AppControl(masterLauncher_t) :
			TopLevelSerializableUnit(Engine::Serialize::MADITOR)
		{
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
