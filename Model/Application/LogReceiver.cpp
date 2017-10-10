#include "maditormodellib.h"

#include "ModuleLoader.h"

#include "Model\Project\Project.h"

#include "Model\Project\Module.h"

#include "Model\Project\ModuleList.h"

#include "ApplicationLauncher.h"
#include "LogReceiver.h"

#include "Logs\LogTableModel.h"

namespace Maditor {
	namespace Model {

		LogReceiver::LogReceiver(Engine::Serialize::SerializableUnitBase *parent, LogTableModel * model, const std::list<std::string> &textLogs) :
			SerializableUnit(parent),
			mModel(model)
		{

		}

		LogReceiver::~LogReceiver()
		{
		}

		void LogReceiver::receiveImpl(const std::string & msg, Engine::Util::MessageType level, const std::string & logName, const std::string & fullTraceback, const std::string & fileName, int lineNr)
		{
			mModel->addMessage(msg.c_str(), level, logName.c_str(), fullTraceback.c_str(), fileName, lineNr);
		}

}
}