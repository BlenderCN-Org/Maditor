#pragma once

#include "Serialize\serializableunit.h"
#include "Serialize\Container\observed.h"
#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT StatsModel : public QObject, public Engine::Serialize::SerializableUnitBase {
			Q_OBJECT
		public:
			StatsModel();

			void reset();

			virtual void timerEvent(QTimerEvent *evt) override;

			void setProcess(HANDLE handle);

			void trackAllocations();
			void logTrackedAllocations();

		protected:
			void dummy();			

		signals:
			void averageFPSChanged(float fps);
			void memoryUsageChanged(size_t mem);
			void ogreMemoryUsageChanged(size_t mem);

		private:
			Engine::Serialize::Observed<size_t> mOgreMemoryUsage;
			
			Engine::Serialize::Action<decltype(&StatsModel::dummy), &StatsModel::dummy, Engine::Serialize::ActionPolicy::request> startTrack, stopTrack;			
			
			Engine::Serialize::Observed<float> mAverageFPS;		
			

			int mTimerId;
			HANDLE mHandle;
			size_t mCurrentUsage;

		};
	}
}