#pragma once

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost\interprocess\ipc\message_queue.hpp>

namespace Maditor {
	namespace Shared {


		typedef boost::interprocess::allocator<char,
			boost::interprocess::managed_shared_memory::segment_manager> SharedCharAllocator;
		typedef boost::interprocess::basic_string<char, std::char_traits<char>, SharedCharAllocator> SharedString;

		typedef boost::interprocess::allocator<boost::interprocess::offset_ptr<BoostIPCConnection>,
			boost::interprocess::managed_shared_memory::segment_manager> ConnectionPtrAllocator;
		typedef boost::interprocess::list<boost::interprocess::offset_ptr<BoostIPCConnection>, ConnectionPtrAllocator> SharedConnectionQueue;


	}
}



