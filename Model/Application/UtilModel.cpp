#include "maditormodellib.h"

#include "UtilModel.h"

namespace Maditor {
	namespace Model {



		UtilModel::UtilModel() :
			TopLevelSerializableUnit(Engine::Serialize::UTIL),
			mProfiler(this)
		{
		}

		ProfilerModel * UtilModel::profiler()
		{
			return mProfiler.ptr();
		}

		void UtilModel::reset()
		{
			mProfiler->reset();
		}

	}
}