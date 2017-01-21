
#include "maditorlauncherlib.h"

#include "LogListener.h"


namespace Maditor {
	namespace Launcher {

		LogListener::LogListener() :
		 receiveMessage(this, &LogListener::receiveImpl)
		{

		}

		LogListener::~LogListener() {

		}

		void LogListener::receiveImpl(const std::string & msg, int level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
		}

		void LogListener::messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage)
		{
			std::stringstream fullTraceback;
			std::string fileName;
			int lineNr = -2;

			if (logName != "Ogre.log") {				
				const Engine::Util::TraceBack *last = 0;
				for (const Engine::Util::TraceBack &t : Engine::Util::UtilMethods::traceBack()) {
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
			}

			receiveMessage(message.c_str(), lml, logName.c_str(), fullTraceback.str(), fileName, lineNr);
		}

	}
}
