#pragma once

#include "Serialize\Container\action.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {

		class OgreLogReader : public Engine::Serialize::SerializableUnit {

		public:
			OgreLogReader(LogsModel *model, const std::list<std::string> &textLogs);
			virtual ~OgreLogReader();

			void clear();

		private:
			void receiveImpl(const std::string &msg, Util::MessageType level, const std::string &logName, const std::string &fullTraceback, const std::string &fileName, int lineNr);
			

		private:
			
			LogsModel *mModel;

			std::map<std::string, Log*> mLogs;
			
			Engine::Serialize::Action<decltype(&OgreLogReader::receiveImpl), &OgreLogReader::receiveImpl, Engine::Serialize::ActionPolicy::notification> receiveMessage;

			std::list<std::string> mTextLogs;

		};
	}
}