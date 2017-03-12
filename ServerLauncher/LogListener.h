#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"
#include "Util\UtilMethods.h"

namespace Maditor {
	namespace Launcher {

class LogListener : public Engine::Serialize::SerializableUnitBase, public Util::LogListener {

public:
	LogListener();
	~LogListener();

protected:
	// Inherited via LogListener
	virtual void messageLogged(const std::string & message, Util::MessageType lml, const std::list<Util::TraceBack> &traceback, const std::string & logName) override;


private:
	void receiveImpl(const std::string &msg, Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);

private:
	Engine::Serialize::Action<decltype(&LogListener::receiveImpl), &LogListener::receiveImpl, Engine::Serialize::ActionPolicy::notification> receiveMessage;



};

	}
}
