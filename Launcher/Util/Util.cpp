#include "madginelib.h"

#include "Util.h"
#include "Util/UtilMethods.h"

#include "TopLevelIds.h"

namespace Maditor {
	namespace Launcher {

		Util::Util()
		{
		}

		void Util::setApp(Engine::App::Application * app)
		{
			mStats->setApp(app);
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