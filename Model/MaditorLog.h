#pragma once

#include "Logs\log.h"
#include "Util\UtilMethods.h"

namespace Maditor {
	namespace Model {


		class MaditorLog : public Util::Log, public Log {
		public:
			MaditorLog();

			// Inherited via Log
			virtual void log(const std::string & msg, Util::MessageType lvl, const std::list<Util::TraceBack>& traceBack = {}) override;
		};

	}
}