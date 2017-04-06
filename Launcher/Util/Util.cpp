#include "madginelib.h"

#include "Util.h"
#include "Util/UtilMethods.h"

#include "TopLevelIds.h"

namespace Maditor {
	namespace Launcher {

		Util::Util(Engine::App::Application *app) :			
			mStats(app)
		{
			Engine::Util::UtilMethods::setup(&mLog);
		}

		void Util::setup()
		{
			mLog.setup("Madgine.log");
		}

		void Util::shutdown()
		{
			mLog.shutdown();
		}

		Engine::Util::Profiler * Util::profiler()
		{
			return mProfiler.ptr();
		}

		void Util::update()
		{
			mProfiler->update();
			mStats->update();
		}

	}
}