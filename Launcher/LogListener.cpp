
#include "maditorlauncherlib.h"

#include "LogListener.h"


namespace Maditor {
	namespace Launcher {

		LogListener::LogListener()
		{

		}

		LogListener::~LogListener() {

		}

		void LogListener::receiveImpl(const std::string & msg, Engine::Util::MessageType level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
		}

		void LogListener::messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage)
		{
			std::stringstream fullTraceback;
			std::string fileName;
		
			int lineNr = -1;

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
			

			Engine::Util::MessageType type;
			switch (lml) {
			case Ogre::LML_CRITICAL:
				type = Engine::Util::ERROR_TYPE;
				break;
			case Ogre::LML_NORMAL:
				type = Engine::Util::WARNING_TYPE;
				break;
			case Ogre::LML_TRIVIAL:
				type = Engine::Util::LOG_TYPE;
				break;
			default:
				throw 0;
			}

			receiveMessage(message.c_str(), type, logName.c_str(), fullTraceback.str(), fileName, lineNr, {});
		}

	}
}
