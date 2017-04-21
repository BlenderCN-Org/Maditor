#pragma once

#include "Serialize\Streams\buffered_streambuf.h"

#include "boostIPCConnection.h"

namespace Maditor {
	namespace Shared {


		class MADITOR_SHARED_EXPORT BoostIPCBuffer : 
			public Engine::Serialize::buffered_streambuf
		{

		public:
			BoostIPCBuffer(SharedConnectionPtr &&conn, bool slave);
			BoostIPCBuffer(const BoostIPCBuffer &) = delete;
			BoostIPCBuffer(BoostIPCBuffer &&other);
			virtual ~BoostIPCBuffer();

			virtual bool isClosed() override;

		protected:
			// Geerbt über buffered_streambuf
			virtual bool handleError() override;

			virtual size_t rec(char *, size_t) override;

			virtual size_t send(char *, size_t) override;

			virtual void close() override;

		private:
			SharedConnectionPtr mConnection;

			boost::interprocess::message_queue mReadQueue, mWriteQueue;

			static const constexpr size_t sMaxMessageSize = 256;

			enum {
				WOULD_BLOCK,
				SIZE_MISMATCH
			} mError;

			bool mSlave;


		};

	}
}

