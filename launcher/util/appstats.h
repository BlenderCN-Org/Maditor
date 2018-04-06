#pragma once

#include "Madgine/Serialize\serializableunit.h"
#include "Madgine/Serialize\Container\observed.h"
#include "Madgine/Serialize\Container\action.h"



namespace Maditor {
	namespace Launcher {

		class AppStats : public Engine::Serialize::SerializableUnit<AppStats> {
		public:
			AppStats();

			void setApp(Engine::App::Application *app);
			
			void update();			

		private:
			Engine::Serialize::Observed<float> mAverageFPS;
			Engine::App::Application *mApp;

		};

	}
}
