#pragma once

#include "Util/UtilMethods.h"

namespace Maditor {
	namespace Launcher {


		class OgreUtilLog : public Engine::Util::Log {
		public:
			OgreUtilLog();
			virtual ~OgreUtilLog();

			void setup(const std::string &name);
			void shutdown();

			// Inherited via Log
			virtual void log(const std::string & msg, Engine::Util::MessageType lvl, const std::list<Engine::Util::TraceBack>& traceBack = {}) override;
			virtual std::string getName() override;

		private:
			Ogre::Log *mLog;
			
		};

	}
}