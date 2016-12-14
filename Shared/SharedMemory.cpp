
#include "maditorsharedlib.h"

#include "SharedMemory.h"

#include <boost\interprocess\shared_memory_object.hpp>

namespace Maditor {
	namespace Shared {

		SharedMemory::SharedMemory(create_t) :
			mMemory((boost::interprocess::shared_memory_object::remove("Maditor_Memory"), boost::interprocess::create_only), "Maditor_Memory", 1000000),
			mCreate(true)
		{
			mData = mMemory.construct<Shared>("SharedData")(mMemory.get_segment_manager());
		}

		SharedMemory::SharedMemory(open_t) :
			mMemory(boost::interprocess::open_only, "Maditor_Memory"),
			mCreate(false)
		{
			mData = mMemory.find<Shared>("SharedData").first;
		}
		SharedMemory::~SharedMemory() {
			if (mCreate)
				boost::interprocess::shared_memory_object::remove("Maditor_Memory");
		}
		boost::interprocess::managed_shared_memory::segment_manager * SharedMemory::mgr() {
			return mMemory.get_segment_manager();
		}
		Shared & SharedMemory::data() {
			return *mData;
		}
		Shared::Shared(boost::interprocess::managed_shared_memory::segment_manager * mgr) :
			mAppInfo(mgr),
			mInput(mgr)
		{}
	}
}