#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"
#include "Util\UtilMethods.h"
#include "signalslot/slot.h"

namespace Maditor {
	namespace Launcher {

class LogListener : public Engine::Serialize::SerializableUnit<LogListener>
	, public Engine::Util::LogListener
#ifdef MADGINE_CLIENT_BUILD
	, public Ogre::LogListener 
#endif
{


public:
	LogListener(Engine::Serialize::TopLevelSerializableUnitBase *topLevel);
	~LogListener();

	void init();

protected:
	// Inherited via LogListener
	virtual void messageLogged(const std::string & message, Engine::Util::MessageType lml, const std::list<Engine::Util::TraceBack> &traceback, const std::string & logName) override;

#ifdef MADGINE_CLIENT_BUILD
	virtual void messageLogged(const Ogre::String & message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & logName, bool & skipThisMessage) override;
#endif
private:
	void receiveImpl(const std::string &msg, Engine::Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);

private:
	Engine::Serialize::Action<decltype(&LogListener::receiveImpl), &LogListener::receiveImpl, Engine::Serialize::ActionPolicy::notification> receiveMessage;
	typedef Engine::SignalSlot::Slot<decltype(&decltype(LogListener::receiveMessage)::operator()), &decltype(LogListener::receiveMessage)::operator()> SlotType;
	std::unique_ptr<SlotType> mSlot;
	
};

	}
}
