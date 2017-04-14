#include "maditorsharedlib.h"

#include "boostIPCbuffer.h"

#include "boostIPCConnection.h"

namespace Maditor {
	namespace Shared {

		BoostIPCBuffer::BoostIPCBuffer(BoostIPCConnection *conn, const std::string &prefix, bool slave) :
			mReadQueue(boost::interprocess::open_or_create, (prefix + (slave ? "master" : "slave")).c_str(), 128, sMaxMessageSize),
			mWriteQueue(boost::interprocess::open_or_create, (prefix + (slave ? "slave" : "master")).c_str(), 128, sMaxMessageSize),
			mPrefix(prefix),
			mSlave(slave),
			mConnection(conn)
		{
		}


		BoostIPCBuffer::BoostIPCBuffer(BoostIPCBuffer && other) :
			buffered_streambuf(std::forward<BoostIPCBuffer>(other)),
			mReadQueue(boost::interprocess::open_only, (other.mPrefix + (other.mSlave ? "master" : "slave")).c_str()),
			mWriteQueue(boost::interprocess::open_only, (other.mPrefix + (other.mSlave ? "slave" : "master")).c_str()),
			mPrefix(other.mPrefix),
			mSlave(other.mSlave),
			mConnection(other.mConnection)
		{
			other.mConnection = nullptr;
			other.close();
		}

		BoostIPCBuffer::~BoostIPCBuffer()
		{
			if (mConnection)
				mConnection->close();
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
			size_t receivedSize = 0;
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
				if (mConnection->count() == 1)
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
				if (mConnection->count() == 1)
					return 0;
				mError = WOULD_BLOCK;
				return -1;
			}
		}

		void BoostIPCBuffer::close()
		{
			buffered_streambuf::close();
			if (mConnection) {
				mConnection->close();
				mConnection = nullptr;
			}
			boost::interprocess::message_queue::remove((mPrefix + "master").c_str());
			boost::interprocess::message_queue::remove((mPrefix + "slave").c_str());
		}

	}
}
