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

		void OgreLogReader::receiveImpl(const std::string & msg, Engine::Util::MessageType level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
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
			
			log->log(msg, level, fullTraceback, fileName, lineNr);
		}

}
}