
#include "madginelib.h"

#include "OgreUtilLog.h"

namespace Maditor {
	namespace Launcher {		



		OgreUtilLog::OgreUtilLog() :
			mLog(nullptr)
		{
			
		}

		OgreUtilLog::~OgreUtilLog()
		{
			
		}

		void OgreUtilLog::setup(const std::string & name)
		{
			mLog = Ogre::LogManager::getSingleton().createLog(name);
			mLog->setLogDetail(Ogre::LL_BOREME);
		}

		void OgreUtilLog::shutdown()
		{
			if (mLog)
				Ogre::LogManager::getSingleton().destroyLog(mLog);
		}

		void OgreUtilLog::log(const std::string & msg, Engine::Util::MessageType lvl, const std::list<Engine::Util::TraceBack>& traceBack)
		{
			Ogre::LogMessageLevel level;
			switch (lvl) {
			case Engine::Util::ERROR_TYPE:
				level = Ogre::LML_CRITICAL;
				break;
			case Engine::Util::WARNING_TYPE:
				level = Ogre::LML_NORMAL;
				break;
			case Engine::Util::LOG_TYPE:
				level = Ogre::LML_TRIVIAL;
				break;
			default:
				throw 0;
			}
			mLog->logMessage(msg.c_str(), level);
		}

		std::string OgreUtilLog::getName()
		{
			return mLog->getName();
		}

	}
}