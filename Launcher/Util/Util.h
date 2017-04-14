#pragma once

#include "Util\Profiler.h"
#include "AppStats.h"

#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\serialized.h"


namespace Maditor {
	namespace Launcher {

		class Util : public Engine::Singleton<Util>, public Engine::Serialize::TopLevelSerializableUnit<Util> {
		public:
			Util();

			void setApp(Engine::App::Application* app);
			
			Engine::Util::Profiler *profiler();

			void update();

		private:

			Engine::Serialize::Serialized<Engine::Util::Profiler> mProfiler;
			Engine::Serialize::Serialized<AppStats> mStats;

		};

	}
}