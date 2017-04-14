#include "madginelib.h"

#include "AppStats.h"

#include <iostream>
#include "App\application.h"

namespace Maditor {
	namespace Launcher {

		AppStats::AppStats()
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