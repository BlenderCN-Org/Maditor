#pragma once

#include "SharedBase.h"

namespace Maditor {
	namespace Shared {


		struct ServerInfo {
			ServerInfo(boost::interprocess::managed_shared_memory::segment_manager *mgr) :
				mMediaDir(SharedCharAllocator(mgr)),
				mProjectDir(SharedCharAllocator(mgr)),
				mServerClass(SharedCharAllocator(mgr)){}

			SharedString mServerClass;
			SharedString mMediaDir;
			SharedString mProjectDir;
			bool mDebugged;
		};


	}
}