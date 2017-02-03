#pragma once

#include "Logs\log.h"
#include "Util\UtilMethods.h"

namespace Maditor {
	namespace Model {


		class MaditorLog : public Engine::Util::Log, public Log {
		public:
			MaditorLog();

			// Inherited via Log
			virtual void log(const std::string & msg, Engine::Util::MessageType lvl, const std::list<Engine::Util::TraceBack>& traceBack = {}) override;
		};

	}
}