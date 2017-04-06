#pragma once

#include "SharedBase.h"
#include <boost/thread/thread_time.hpp>

namespace Maditor {
	namespace Shared {

		struct ApplicationInfo {
			ApplicationInfo(boost::interprocess::managed_shared_memory::segment_manager *mgr) :
				mMediaDir(SharedCharAllocator(mgr)),
				mProjectDir(SharedCharAllocator(mgr)),
				mAppId(0) {}

			int mWindowWidth;
			int mWindowHeight;
			size_t mWindowHandle;
			SharedString mMediaDir;
			SharedString mProjectDir;
			bool mDebugged;
			size_t mAppId;
			boost::interprocess::interprocess_mutex mAppIdMutex;
			boost::interprocess::interprocess_condition mAppIdCondition;

			void setAppId(size_t id) {
				mAppId = id;
				mAppIdCondition.notify_one();				
			}
			size_t waitForAppId() {
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mAppIdMutex);
				if (!mAppId) {
					if (!mAppIdCondition.timed_wait(lock, boost::get_system_time() + boost::posix_time::milliseconds(1000)))
						return 0;
				}
				return mAppId;
			}
		};


	}
}