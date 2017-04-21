#include "maditorsharedlib.h"

#include "boostIPCConnection.h"

namespace Maditor {
	namespace Shared {
		BoostIPCConnection::BoostIPCConnection(const std::string & prefix, boost::interprocess::managed_shared_memory::segment_manager *mgr) :
			mPrefix(prefix.c_str(), mgr)			
		{
			boost::interprocess::message_queue::remove((mPrefix + "master").c_str());
			boost::interprocess::message_queue::remove((mPrefix + "slave").c_str());
			boost::interprocess::message_queue(boost::interprocess::create_only, (prefix + "master").c_str(), 128, sMaxMessageSize);
			boost::interprocess::message_queue(boost::interprocess::create_only, (prefix + "slave").c_str(), 128, sMaxMessageSize);
		}
		BoostIPCConnection::~BoostIPCConnection()
		{
			boost::interprocess::message_queue::remove((mPrefix + "master").c_str());
			boost::interprocess::message_queue::remove((mPrefix + "slave").c_str());
		}

		const SharedString & BoostIPCConnection::prefix()
		{
			return mPrefix;
		}

	}
}
