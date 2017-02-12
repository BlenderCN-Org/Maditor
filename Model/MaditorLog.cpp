#include "maditormodellib.h"

#include "MaditorLog.h"

namespace Maditor {
	namespace Model {
		MaditorLog::MaditorLog() :
			Model::Log("Maditor.log", Model::Log::GuiLog)
		{
		}

		void MaditorLog::log(const std::string & msg, Util::MessageType lvl, const std::list<Util::TraceBack>& traceBack)
		{			
			Model::Log::log(msg.c_str(), lvl);
		}

		std::string MaditorLog::getName()
		{
			return "Maditor.log";
		}

	}
}

