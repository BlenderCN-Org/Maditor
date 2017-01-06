#pragma once

#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\serialized.h"

#include "ProfilerModel.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT UtilModel : public Engine::Serialize::TopLevelSerializableUnit {
		public:

			UtilModel();
			UtilModel(const UtilModel &) = delete;

			ProfilerModel *profiler();

			void reset();

		private:
			Engine::Serialize::Serialized<ProfilerModel> mProfiler;

		};
	}
}