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

		OgreLogReader::OgreLogReader(LogsModel * model, const std::list<std::string> &textLogs) :
			mModel(model),
			receiveMessage(this),
			mTextLogs(textLogs)
		{

		}

		OgreLogReader::~OgreLogReader()
		{
			clear();
		}

		void OgreLogReader::clear()
		{
			for (const std::pair<const std::string, Log *> &log : mLogs) {
				mModel->removeLog(log.second);
				delete log.second;
			}
			mLogs.clear();
		}

		void OgreLogReader::receiveImpl(const std::string & msg, Util::MessageType level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
			auto it = mLogs.find(logName);

			Log *log;
			if (it == mLogs.end()) {
				log = new Log(logName, std::find(mTextLogs.begin(), mTextLogs.end(), logName) != mTextLogs.end() ? Log::TextLog : Log::GuiLog);
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