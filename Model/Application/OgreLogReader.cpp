#include "maditormodellib.h"

#include "ModuleLoader.h"

#include "Model\Project\Project.h"

#include "Model\Project\Module.h"

#include "Model\Project\ModuleList.h"

#include "ApplicationLauncher.h"
#include "OgreLogReader.h"

#include "Logs\LogsModel.h"
#include "Logs\log.h"

namespace Maditor {
	namespace Model {

		OgreLogReader::OgreLogReader(LogsModel * model) :
			mModel(model),
			receiveMessage(this, &OgreLogReader::receiveImpl)
		{

		}

		void OgreLogReader::receiveImpl(const std::string & msg, Ogre::LogMessageLevel level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
			auto it = mLogs.find(logName);

			Log *log;
			if (it == mLogs.end()) {
				log = new Log(logName, lineNr == -2 ? Log::TextLog : Log::GuiLog);
				mModel->addLog(log);
				mLogs[logName] = log;
			}
			else {
				log = it->second;
			}

			MessageType type;
			switch (level) {
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
			
			log->log(msg, type, fullTraceback, fileName, lineNr);
		}

}
}