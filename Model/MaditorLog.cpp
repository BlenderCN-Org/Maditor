#include "maditormodellib.h"

#include "MaditorLog.h"

namespace Maditor {
	namespace Model {
		MaditorLog::MaditorLog() :
			Ogre::Log("Maditor.log"),
			Model::Log("Maditor.log", Model::Log::GuiLog)
		{
			addListener(this);
		}

		void MaditorLog::messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage)
		{
			MessageType type;
			switch (lml) {
			case Ogre::LML_TRIVIAL:
				type = LOG_TYPE;
				break;
			case Ogre::LML_NORMAL:
				type = WARNING_TYPE;
				break;
			case Ogre::LML_CRITICAL:
				type = ERROR_TYPE;
				break;
			default:
				throw 0;
			}
			log(message.c_str(), type);
		}

	}
}

