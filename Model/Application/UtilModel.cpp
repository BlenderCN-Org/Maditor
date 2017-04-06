#include "maditormodellib.h"

#include "UtilModel.h"

#include "TopLevelIds.h"

namespace Maditor {
	namespace Model {



		UtilModel::UtilModel()
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