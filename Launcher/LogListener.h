#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Launcher {

class LogListener : public Engine::Serialize::SerializableUnit, public Ogre::LogListener {

public:
	LogListener();
	~LogListener();

protected:
	// Inherited via LogListener
	virtual void messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage) override;


private:
	void receiveImpl(const std::string &msg, Engine::Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);

private:
	Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll, const std::string &, Engine::Util::MessageType, const std::string &, const std::string &, const std::string &, int> receiveMessage;



};

	}
}
