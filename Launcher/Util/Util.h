#pragma once

#include "Util\Profiler.h"
#include "AppStats.h"

#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\serialized.h"

#include "OgreUtilLog.h"

namespace Maditor {
	namespace Launcher {

		class Util : public Engine::Singleton<Util>, public Engine::Serialize::TopLevelSerializableUnit<Util> {
		public:
			Util(Engine::App::Application *app);

			void setup();
			void shutdown();

			Engine::Util::Profiler *profiler();

			void update();

		private:
			OgreUtilLog mLog;

			Engine::Serialize::Serialized<Engine::Util::Profiler> mProfiler;
			Engine::Serialize::Serialized<AppStats> mStats;

		};

	}
}