#include "maditormodellib.h"

#include "UtilModel.h"

#include "TopLevelIds.h"

namespace Maditor {
	namespace Model {



		UtilModel::UtilModel(bool server) :
			TopLevelSerializableUnit(Engine::UTIL + (server ? 20 : 0)),
			mProfiler(),
			mStats()
		{
		}

		ProfilerModel * UtilModel::profiler()
		{
			return mProfiler.ptr();
		}

		StatsModel * UtilModel::stats()
		{
			return mStats.ptr();
		}

		void UtilModel::reset()
		{
			mProfiler->reset();
			mStats->reset();
		}

	}
}