#pragma once

#include "Serialize\Container\action.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {

		class LogReceiver : public Engine::Serialize::SerializableUnit<LogReceiver> {

		public:
			LogReceiver(Engine::Serialize::SerializableUnitBase *parent, LogTableModel *model, const std::list<std::string> &textLogs);
			virtual ~LogReceiver();

			void clear();

		private:
			void receiveImpl(const std::string &msg, Engine::Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);
			

		private:
			
			LogTableModel *mModel;

			Engine::Serialize::Action<decltype(&LogReceiver::receiveImpl), &LogReceiver::receiveImpl, Engine::Serialize::ActionPolicy::notification> receiveMessage;

			
		};
	}
}