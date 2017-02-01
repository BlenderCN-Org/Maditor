#pragma once

#include "Logs\log.h"

namespace Maditor {
	namespace Model {


		class MaditorLog : public Ogre::Log, Ogre::LogListener, public Log {
		public:
			MaditorLog();

			// Inherited via LogListener
			virtual void messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage) override;
		};

	}
}