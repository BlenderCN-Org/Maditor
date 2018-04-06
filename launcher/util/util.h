#pragma once

#include "Madgine/Util\Profiler.h"
#include "AppStats.h"

#include "Madgine/Serialize\toplevelserializableunit.h"

#include "Madgine/Serialize\Container\serialized.h"


namespace Maditor {
	namespace Launcher {

		class Util : public Engine::Singleton<Util>, public Engine::Serialize::SerializableUnit<Util> {
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