#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"
#include "Util\UtilMethods.h"

namespace Maditor {
	namespace Launcher {

class LogListener : public Engine::Serialize::SerializableUnit<LogListener>, public Engine::Util::LogListener, public Ogre::LogListener {

public:
	LogListener(Engine::Serialize::TopLevelSerializableUnitBase *topLevel);
	~LogListener();

protected:
	// Inherited via LogListener
	virtual void messageLogged(const std::string & message, Engine::Util::MessageType lml, const std::list<Engine::Util::TraceBack> &traceback, const std::string & logName) override;


	virtual void messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage) override;

private:
	void receiveImpl(const std::string &msg, Engine::Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);

private:
	Engine::Serialize::Action<decltype(&LogListener::receiveImpl), &LogListener::receiveImpl, Engine::Serialize::ActionPolicy::notification> receiveMessage;

};

	}
}
