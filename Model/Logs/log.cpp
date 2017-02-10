#include "maditormodellib.h"

#include "log.h"

#include "LogTableModel.h"

namespace Maditor {
	namespace Model {
		Log::Log(const std::string & name, Type type) :
			mName(name),
			mType(type)
		{
			if (type == GuiLog) {
				mModel = std::make_unique<LogTableModel>();
			}
		}

		Log::~Log()
		{
		}

		Log::Type Log::type() const
		{
			return mType;
		}

		const std::string & Log::name() const
		{
			return mName;
		}

		LogTableModel * Log::model()
		{
			return mModel.get();
		}

		void Log::log(const std::string & msg, Util::MessageType type, const std::string & traceback, const std::string & fileName, int lineNr)
		{
			if (mType == GuiLog) {
				mModel->addMessage(QString::fromStdString(msg), type, QString::fromStdString(traceback), fileName, lineNr);
			}
			emit messageReceived(QString::fromStdString(msg));
		}

	}
}