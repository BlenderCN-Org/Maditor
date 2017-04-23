#include "maditorsharedlib.h"

#include "boostIPCbuffer.h"

#include "boostIPCConnection.h"

namespace Maditor {
	namespace Shared {

		BoostIPCBuffer::BoostIPCBuffer(SharedConnectionPtr && conn, bool slave) :
			mConnection(std::forward<SharedConnectionPtr>(conn)),
			mReadQueue(boost::interprocess::open_only, (mConnection->prefix() + (slave ? "master" : "slave")).c_str()),
			mWriteQueue(boost::interprocess::open_only, (mConnection->prefix() + (slave ? "slave" : "master")).c_str()),
			mSlave(slave)			
		{
		}

		BoostIPCBuffer::BoostIPCBuffer(BoostIPCBuffer && other) :
			buffered_streambuf(std::forward<BoostIPCBuffer>(other)),
			mConnection(std::move(other.mConnection)),
			mReadQueue(boost::interprocess::open_only, (mConnection->prefix() + (other.mSlave ? "master" : "slave")).c_str()),
			mWriteQueue(boost::interprocess::open_only, (mConnection->prefix() + (other.mSlave ? "slave" : "master")).c_str()),
			mSlave(other.mSlave)
		{
			other.close();
		}

		BoostIPCBuffer::~BoostIPCBuffer()
		{
		}

		bool BoostIPCBuffer::isClosed()
		{
			return buffered_streambuf::isClosed();
		}

		bool BoostIPCBuffer::handleError()
		{
			switch (mError) {
			case WOULD_BLOCK:
				return true;
			default:
				return false;
			}
		}

		size_t BoostIPCBuffer::rec(char *buf, size_t len)
		{
			unsigned receivedSize = 0;
			unsigned int priority;
			char buffer[sMaxMessageSize];
			if (mReadQueue.try_receive(buffer, sMaxMessageSize, receivedSize, priority)) {
				if (receivedSize > len) {
					mError = SIZE_MISMATCH;
					return -1;
				}
				std::memcpy(buf, buffer, receivedSize);
				return receivedSize;
			}
			else {
				if (mConnection.use_count() == 1)
					return 0;
				mError = WOULD_BLOCK;
				return -1;
			}

		}

		size_t BoostIPCBuffer::send(char *buf, size_t len)
		{
			if (mWriteQueue.try_send(buf, len, 0))
				return len;
			else {
				if (mConnection.use_count() == 1)
					return 0;
				mError = WOULD_BLOCK;
				return -1;
			}
		}

		void BoostIPCBuffer::close()
		{
			buffered_streambuf::close();
			mConnection.reset();
		}

	}
}
