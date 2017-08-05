#include "madginelib.h"

#include "AppStats.h"

#include "App\application.h"

namespace Maditor {
	namespace Launcher {

		AppStats::AppStats(Engine::Serialize::TopLevelSerializableUnitBase *topLevel) :
			SerializableUnit(topLevel)
		{
		}



		void AppStats::setApp(Engine::App::Application * app)
		{
			mApp = app;
		}

		void AppStats::update()
		{
			mAverageFPS = mApp ? mApp->getFPS() : 0.0f;
		}
		
	}
}