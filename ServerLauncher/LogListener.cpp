
#include "maditorserverlauncherlib.h"

#include "LogListener.h"


namespace Maditor {
	namespace Launcher {

		LogListener::LogListener() :
		 receiveMessage(this)
		{

		}

		LogListener::~LogListener() {

		}

		void LogListener::receiveImpl(const std::string & msg, Util::MessageType level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
		}

		void LogListener::messageLogged(const std::string & message, Util::MessageType lml, const std::list<Util::TraceBack> &traceback, const std::string & logName)
		{
			std::stringstream fullTraceback;
			std::string fileName;
			int lineNr = -2;
			
			const Util::TraceBack *last = 0;
			for (const Util::TraceBack &t : Util::UtilMethods::traceBack()) {
				if (last)
					fullTraceback << std::endl;
				last = &t;
				fullTraceback << /*relative*/t.mFile << "(" << t.mLineNr << "): " << t.mFunction;
			}
			if (last) {
				fileName = last->mFile;
				lineNr = last->mLineNr;
			}
			else {
				lineNr = -1;
			}

			receiveMessage(message, lml, logName, fullTraceback.str(), fileName, lineNr, {});
		}

	}
}
