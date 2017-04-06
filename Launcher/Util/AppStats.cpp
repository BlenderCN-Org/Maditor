#include "madginelib.h"

#include "AppStats.h"

#include <iostream>
#include "App\application.h"

namespace Maditor {
	namespace Launcher {

		AppStats::AppStats(Engine::App::Application *app) :
			mApp(app)
		{
		}



		void AppStats::update()
		{
			mAverageFPS = mApp->getFPS();
		}
		
	}
}