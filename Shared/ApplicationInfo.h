#pragma once

#include "SharedBase.h"

namespace Maditor {
	namespace Shared {

		struct ApplicationInfo {
			ApplicationInfo(boost::interprocess::managed_shared_memory::segment_manager *mgr) :
				mMediaDir(SharedCharAllocator(mgr)),
				mProjectDir(SharedCharAllocator(mgr)) {}

			int mWindowWidth;
			int mWindowHeight;
			size_t mWindowHandle;
			SharedString mMediaDir;
			SharedString mProjectDir;
			bool mDebugged;
		};


	}
}