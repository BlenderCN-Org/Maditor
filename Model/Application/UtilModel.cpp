#include "maditormodellib.h"

#include "UtilModel.h"

namespace Maditor {
	namespace Model {



		UtilModel::UtilModel(Engine::Serialize::SerializableUnitBase *parent) :
			SerializableUnit(parent)
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