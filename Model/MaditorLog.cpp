#include "maditormodellib.h"

#include "MaditorLog.h"

namespace Maditor {
	namespace Model {
		MaditorLog::MaditorLog() :
			Model::Log("Maditor.log", Model::Log::GuiLog)
		{
		}

		void MaditorLog::log(const std::string & msg, Engine::Util::MessageType lvl, const std::list<Engine::Util::TraceBack>& traceBack)
		{			
			Model::Log::log(msg.c_str(), lvl);
		}

	}
}

