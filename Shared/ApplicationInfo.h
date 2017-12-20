#pragma once

#include "SharedBase.h"
#include <boost/thread/thread_time.hpp>
#include "Serialize/container/serialized.h"
#include "Serialize/serializable.h"

namespace Maditor {
	namespace Shared {

		
			enum LauncherType {
				CLIENT_LAUNCHER = 0,
				SERVER_LAUNCHER = 1
			};

		struct ApplicationInfo : public Engine::Serialize::Serializable{
			ApplicationInfo() :
			Serializable(true)			
			{}

			virtual void readState(Engine::Serialize::SerializeInStream& in) override
			{
				in >> mDebugged >> mDataDir >> mProjectDir >> mAppName >> mType >> mWindowWidth >> mWindowHeight >> mWindowHandle >> mServerClass;
			}

			virtual void writeState(Engine::Serialize::SerializeOutStream& out) const override
			{
				out << mDebugged << mDataDir << mProjectDir << mAppName << mType << mWindowWidth << mWindowHeight << mWindowHandle << mServerClass;
			}

			//General Info
			bool mDebugged;
			std::string mDataDir;
			std::string mProjectDir;
			std::string mAppName;
			LauncherType mType;
			
			//Client
			int mWindowWidth;
			int mWindowHeight;
			size_t mWindowHandle;

			//Server
			std::string mServerClass;
			
		};


	}
}