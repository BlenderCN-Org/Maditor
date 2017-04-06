#pragma once

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

typedef boost::interprocess::allocator<char,
	boost::interprocess::managed_shared_memory::segment_manager> SharedCharAllocator;
typedef boost::interprocess::basic_string<char, std::char_traits<char>, SharedCharAllocator> SharedString;

