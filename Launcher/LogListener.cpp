
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

		void LogListener::receiveImpl(const std::string & msg, Util::MessageType level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
		}

		void LogListener::messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage)
		{
			std::stringstream fullTraceback;
			std::string fileName;
		
			int lineNr = -1;

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
			

			Util::MessageType type;
			switch (lml) {
			case Ogre::LML_CRITICAL:
				type = Util::ERROR_TYPE;
				break;
			case Ogre::LML_NORMAL:
				type = Util::WARNING_TYPE;
				break;
			case Ogre::LML_TRIVIAL:
				type = Util::LOG_TYPE;
				break;
			default:
				throw 0;
			}

			receiveMessage(message.c_str(), type, logName.c_str(), fullTraceback.str(), fileName, lineNr);
		}

	}
}
