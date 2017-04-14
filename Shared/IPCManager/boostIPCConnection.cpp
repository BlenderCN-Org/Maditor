#include "maditorsharedlib.h"

#include "boostIPCConnection.h"

namespace Maditor {
	namespace Shared {
		
		BoostIPCConnection::BoostIPCConnection() :
			mCount(2)
		{
		}

		void BoostIPCConnection::close()
		{
			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mMutex);
			--mCount;
		}

		int BoostIPCConnection::count()
		{
			return mCount;
		}

	}
}
