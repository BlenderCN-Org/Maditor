
#include "maditorsharedlib.h"

#include "ServerSharedMemory.h"

#include "ServerInfo.h"

#include <boost\interprocess\shared_memory_object.hpp>

namespace Maditor {
	namespace Shared {

		ServerSharedMemory::ServerSharedMemory(create_t) :
			mMemory((boost::interprocess::shared_memory_object::remove("Maditor_Server_Memory"), boost::interprocess::create_only), "Maditor_Server_Memory", 1000000),
			mCreate(true)
		{
			mData = mMemory.construct<ServerInfo>("ServerSharedData")(mMemory.get_segment_manager());
		}

		ServerSharedMemory::ServerSharedMemory(open_t) :
			mMemory(boost::interprocess::open_only, "Maditor_Server_Memory"),
			mCreate(false)
		{
			mData = mMemory.find<ServerInfo>("ServerSharedData").first;
		}
		ServerSharedMemory::~ServerSharedMemory() {
			if (mCreate)
				boost::interprocess::shared_memory_object::remove("Maditor_Server_Memory");
		}
		boost::interprocess::managed_shared_memory::segment_manager * ServerSharedMemory::mgr() {
			return mMemory.get_segment_manager();
		}
		ServerInfo & ServerSharedMemory::data() {
			return *mData;
		}
		
	}
}