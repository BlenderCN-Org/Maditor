#pragma once

#include "SharedBase.h"

namespace Maditor {
	namespace Shared {



		class MADITOR_SHARED_EXPORT BoostIPCConnection
		{
		public:
			BoostIPCConnection();
			
			void close();
			int count();

		private:
			int mCount;
			boost::interprocess::interprocess_mutex mMutex;
		};

		

	}
}

