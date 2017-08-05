#pragma once

#include "SharedBase.h"
#include <boost/thread/thread_time.hpp>

namespace Maditor {
	namespace Shared {

		struct ApplicationInfo {
			ApplicationInfo(boost::interprocess::managed_shared_memory::segment_manager *mgr) :
				mMediaDir(SharedCharAllocator(mgr)),
				mProjectDir(SharedCharAllocator(mgr)),
				mServerClass(SharedCharAllocator(mgr)),
				mAppName(SharedCharAllocator(mgr))
			{}

			//General Info
			bool mDebugged;
			SharedString mMediaDir;
			SharedString mProjectDir;
			SharedString mAppName;
			
			//Client
			int mWindowWidth;
			int mWindowHeight;
			size_t mWindowHandle;

			//Server
			SharedString mServerClass;
			
		};


	}
}