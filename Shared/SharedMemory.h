#pragma once

#include "ApplicationInfo.h"
#include "InputInfo.h"

#include <boost\interprocess\managed_shared_memory.hpp>


namespace Maditor {
	namespace Shared {


		struct Shared {
			Shared(boost::interprocess::managed_shared_memory::segment_manager *mgr);

			ApplicationInfo mAppInfo;
			InputShared mInput;
		};


class MADITOR_SHARED_EXPORT SharedMemory {
public:

	struct create_t {};
	static constexpr create_t create = create_t();
	struct open_t {};
	static constexpr open_t open = open_t();

	SharedMemory(create_t);
	SharedMemory(open_t);

	~SharedMemory();

	boost::interprocess::managed_shared_memory::segment_manager *mgr();

	Shared &data();

private:
	Shared *mData;

	boost::interprocess::managed_shared_memory mMemory;

	bool mCreate;
};


	}
}
