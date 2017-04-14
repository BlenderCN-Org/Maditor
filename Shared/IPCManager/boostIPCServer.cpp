#include "maditorsharedlib.h"

#include "boostIPCServer.h"

#include <boost\date_time\posix_time\posix_time.hpp>

namespace Maditor {
	namespace Shared {


		
		BoostIPCServer::BoostIPCServer(boost::interprocess::managed_shared_memory::segment_manager * mgr) :
			mQueue(mgr)
		{
		}

		void BoostIPCServer::enqueue(BoostIPCConnection * conn, int timeout)
		{
			mQueue.emplace_back(conn);
		}

		BoostIPCConnection * BoostIPCServer::poll(int timeout)
		{

			std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();

			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock = (timeout == 0 ?
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex>(mMutex, boost::posix_time::second_clock::local_time() + boost::posix_time::milliseconds(timeout)) :
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex>(mMutex));

			if (lock) {
				while (mQueue.empty()) {
					if (std::chrono::duration_cast<std::chrono::milliseconds>
						(std::chrono::high_resolution_clock::now() - start).count() > timeout) {
						return nullptr;
					}
				}
				BoostIPCConnection *conn = mQueue.front().get();
				mQueue.pop_front();
				return conn;
			}
			else {
				return nullptr;
			}
		}

	}
}
