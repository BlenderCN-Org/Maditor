#pragma once

#include "Serialize\serializableunit.h"
#include "Serialize\Container\observed.h"
#include "Serialize\Container\action.h"



namespace Maditor {
	namespace Launcher {

		class AppStats : public Engine::Serialize::SerializableUnit<AppStats> {
		public:
			AppStats(Engine::Serialize::SerializableUnitBase *parent);

			void setApp(Engine::App::Application *app);
			
			void update();			

		private:
			Engine::Serialize::Observed<float> mAverageFPS;
			Engine::App::Application *mApp;

		};

	}
}
