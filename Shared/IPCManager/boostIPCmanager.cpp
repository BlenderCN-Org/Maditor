#include "maditorsharedlib.h"

#include "boostIPCmanager.h"

#include "Serialize\toplevelserializableunit.h"

#include "SharedMemory.h"

#include "boostIPCstream.h"

#include "boostIPCServer.h"

#include "boostIPCConnection.h"

namespace Maditor {
	namespace Shared {

		//int NetworkManager::sManagerCount = 0;

		BoostIPCManager::BoostIPCManager(SharedMemory *mem) :
			mServer(nullptr),
			mIsServer(false),
			mSlaveStream(nullptr),
			mMemory(mem)
		{
		}

		BoostIPCManager::BoostIPCManager(BoostIPCManager &&other) :
			SerializeManager(std::forward<BoostIPCManager>(other)),
			mServer(other.mServer),
			mIsServer(other.mIsServer),
			mSlaveStream(nullptr),
			mMemory(other.mMemory)
		{
			for (std::pair<const Engine::Serialize::ParticipantId, BoostIPCStream> &stream : other.mStreams)
				if (!stream.second.isClosed())
					other.moveConnection(stream.first, this);
			other.mServer = nullptr;
			other.mIsServer = false;
			if (other.mSlaveStream) {
				mSlaveStream = new BoostIPCStream(std::move(*other.mSlaveStream), *this);
				setSlaveStream(mSlaveStream, false);
				delete other.mSlaveStream;
				other.mSlaveStream = nullptr;
			}
		}

		BoostIPCManager::~BoostIPCManager()
		{
			close();
		}

		bool BoostIPCManager::startServer()
		{
			if (mServer)
				return false;

			mIsServer = true;

			mServer = mMemory->mgr()->construct<BoostIPCServer>("Server")(mMemory->mgr());

			if (!mServer) {
				return false;
			}
						
			return true;
		}

		bool BoostIPCManager::connect(int timeout)
		{
			bool result = connectImpl(timeout);
			mConnectionResult.emit(result);
			return result;
		}

		/*void BoostIPCManager::connect_async(const std::string & url, int portNr, int timeout)
		{
			std::thread(&NetworkManager::connect, this, url, portNr, timeout).detach();
		}*/

		void BoostIPCManager::close()
		{
			if (mServer) {
				removeAllStreams();
				if (mIsServer) {
					mMemory->mgr()->destroy<BoostIPCServer>("Server");
					mIsServer = false;
				}
				assert(mStreams.empty() && mSlaveStream == 0);
				mServer = nullptr;
			}
		}


		void BoostIPCManager::acceptConnections()
		{
			if (mServer) {
				if (mIsServer) {
					while (SharedConnectionPtr conn = mServer->poll()) {
						BoostIPCStream &stream = addMasterStream(BoostIPCStream(std::move(conn), false, *this, createStreamId()));
					}
				}
			}
		}

		bool BoostIPCManager::acceptConnection(int timeout) {
			if (mServer) {
				if (mIsServer) {
					if (SharedConnectionPtr conn = mServer->poll(timeout)) {
						BoostIPCStream &stream = addMasterStream(BoostIPCStream(std::move(conn), false, *this, createStreamId()));
						if (!sendAllMessages(&stream, timeout)) {
							removeMasterStream(&stream);
							return false;
						}
						else {
							return true;
						}
					}
				}
			}
			return false;
		}

		int BoostIPCManager::clientCount()
		{
			return mStreams.size();
		}

		bool BoostIPCManager::isConnected()
		{
			return mServer != nullptr;
		}

		void BoostIPCManager::moveConnection(Engine::Serialize::ParticipantId id, BoostIPCManager * to)
		{
			auto it = mStreams.find(id);
			BoostIPCStream &stream = to->addMasterStream(std::forward<BoostIPCStream>(it->second), false);
			std::list<Engine::Serialize::TopLevelSerializableUnitBase*> newTopLevels;
			std::set_difference(to->getTopLevelUnits().begin(), to->getTopLevelUnits().end(), getTopLevelUnits().begin(), getTopLevelUnits().end(), std::back_inserter(newTopLevels));
			for (Engine::Serialize::TopLevelSerializableUnitBase *newTopLevel : newTopLevels) {
				sendState(stream, newTopLevel);
			}
		}

		void BoostIPCManager::removeSlaveStream()
		{
			if (mSlaveStream) {
				SerializeManager::removeSlaveStream();
				delete mSlaveStream;
				mSlaveStream = nullptr;
				mServer = nullptr;
			}
		}

		void BoostIPCManager::removeMasterStream(Engine::Serialize::BufferedInOutStream * stream)
		{
			SerializeManager::removeMasterStream(stream);
			assert(mStreams.erase(stream->id()) == 1);
		}

		BoostIPCStream &BoostIPCManager::addMasterStream(BoostIPCStream && stream, bool sendState)
		{
			auto res = mStreams.try_emplace(stream.id(), std::forward<BoostIPCStream>(stream), *this);
			if (!res.second)
				throw 0;
			SerializeManager::addMasterStream(&res.first->second, sendState);
			return res.first->second;
		}

		bool BoostIPCManager::connectImpl(int timeout)
		{

			if (mServer)
				return false;

			std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();
			mServer = mMemory->mgr()->find<BoostIPCServer>("Server").first;
			while (!mServer) {
				if (std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::high_resolution_clock::now() - start).count() > timeout) {
					return false;
				}
				mServer = mMemory->mgr()->find<BoostIPCServer>("Server").first;
			}

			SharedConnectionPtr conn = boost::interprocess::make_managed_shared_ptr(
				mMemory->mgr()->construct<BoostIPCConnection>(boost::interprocess::anonymous_instance)(mMemory->uniqueName(), mMemory->mgr()), mMemory->memory());
			mServer->enqueue(conn, timeout);

			mSlaveStream = new BoostIPCStream(std::move(conn), true, *this);
			if (setSlaveStream(mSlaveStream, true, timeout) != Engine::Serialize::NO_ERROR) {
				delete mSlaveStream;
				mSlaveStream = 0;
				mServer = nullptr;
				return false;
			}


			return true;
		}

	}
}
