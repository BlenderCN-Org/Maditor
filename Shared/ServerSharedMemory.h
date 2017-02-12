#pragma once

#include "SharedBase.h"


namespace Maditor {
	namespace Shared {



class MADITOR_SHARED_EXPORT ServerSharedMemory {
public:

	struct create_t {};
	static constexpr create_t create = create_t();
	struct open_t {};
	static constexpr open_t open = open_t();

	ServerSharedMemory(create_t);
	ServerSharedMemory(open_t);

	~ServerSharedMemory();

	boost::interprocess::managed_shared_memory::segment_manager *mgr();

	ServerInfo &data();

private:
	ServerInfo *mData;

	boost::interprocess::managed_shared_memory mMemory;

	bool mCreate;
};


	}
}
