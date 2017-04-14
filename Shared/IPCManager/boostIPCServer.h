#pragma once



#include "SharedBase.h"

namespace Maditor {
	namespace Shared {


		class MADITOR_SHARED_EXPORT BoostIPCServer
		{
		public:
			BoostIPCServer(boost::interprocess::managed_shared_memory::segment_manager *mgr);

			void enqueue(BoostIPCConnection *conn, int timeout);
			BoostIPCConnection *poll(int timeout = 0);

		private:
			SharedConnectionQueue mQueue;
			boost::interprocess::interprocess_mutex mMutex;
			
		};

		

	}
}

