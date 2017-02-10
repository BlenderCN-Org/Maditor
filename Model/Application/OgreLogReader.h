#pragma once

#include "Serialize\Container\action.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {

		class OgreLogReader : public Engine::Serialize::SerializableUnit {

		public:
			OgreLogReader(LogsModel *model);
			
		private:
			void receiveImpl(const std::string &msg, Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);
			

		private:
			
			LogsModel *mModel;

			std::map<std::string, Log*> mLogs;
			
			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll, const std::string &, Util::MessageType, const std::string &, const std::string &, const std::string &, int> receiveMessage;

		};
	}
}