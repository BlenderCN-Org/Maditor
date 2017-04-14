#pragma once

#include "Serialize/Streams/bufferedstream.h"
#include "boostIPCbuffer.h"


namespace Maditor {
	namespace Shared {



		class MADITOR_SHARED_EXPORT BoostIPCStream : 
			public Engine::Serialize::BufferedInOutStream
			 {

		public:
			BoostIPCStream(BoostIPCConnection *conn, const std::string &prefix, bool slave, Engine::Serialize::SerializeManager &mgr, Engine::Serialize::ParticipantId id = 0);
			BoostIPCStream(const BoostIPCStream &) = delete;
			BoostIPCStream(BoostIPCStream &&other, Engine::Serialize::SerializeManager &mgr);
			virtual ~BoostIPCStream();


		private:

			BoostIPCBuffer mBuffer;			

		};

		

	}
}

