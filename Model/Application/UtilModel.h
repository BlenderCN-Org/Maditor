#pragma once

#include "Serialize\serializableunit.h"

#include "Serialize\Container\serialized.h"

#include "ProfilerModel.h"
#include "StatsModel.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT UtilModel : public Engine::Serialize::SerializableUnit<UtilModel> {
		public:

			UtilModel();
			UtilModel(const UtilModel &) = delete;

			ProfilerModel *profiler();
			StatsModel *stats();

			void reset();

		private:
			Engine::Serialize::Serialized<ProfilerModel> mProfiler;
			Engine::Serialize::Serialized<StatsModel> mStats;

		};
	}
}