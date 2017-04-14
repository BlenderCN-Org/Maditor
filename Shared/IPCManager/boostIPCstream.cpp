#include "maditorsharedlib.h"

#include "boostIPCstream.h"

namespace Maditor {
	namespace Shared {

		BoostIPCStream::BoostIPCStream(BoostIPCConnection *conn, const std::string &prefix, bool slave, Engine::Serialize::SerializeManager &mgr, Engine::Serialize::ParticipantId id) :
			Stream(mgr, id),
			BufferedInOutStream(mBuffer, mgr, id),
			mBuffer(conn, prefix, slave)			
		{
			
		}

		BoostIPCStream::BoostIPCStream(BoostIPCStream && other, Engine::Serialize::SerializeManager & mgr) :
			Stream(mgr, other.mId),
			BufferedInOutStream(mBuffer, mgr, other.mId),
			mBuffer(std::forward<BoostIPCBuffer>(other.mBuffer))
		{
			
		}

		BoostIPCStream::~BoostIPCStream()
		{
			
		}



		
	}
}
