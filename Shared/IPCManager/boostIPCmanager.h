#pragma once

#include "Serialize\serializemanager.h"
#include "boostIPCstream.h"


namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT BoostIPCManager : public Engine::Serialize::SerializeManager{

		public:
			BoostIPCManager(SharedMemory *memory);
			BoostIPCManager(const BoostIPCManager &) = delete;
			BoostIPCManager(BoostIPCManager &&);
			virtual ~BoostIPCManager();


			void operator=(const BoostIPCManager &) = delete;

			bool startServer();
			bool connect(int timeout = 0);
			//void connect_async(const std::string &url, int portNr, int timeout = 0);

			void close();


			bool acceptConnection(int timeout);
			void acceptConnections();

			int clientCount();

			bool isConnected();

			void moveConnection(Engine::Serialize::ParticipantId id, BoostIPCManager *to);

			template <class T>
			void connectConnectionSlot(T &slot) {
				mConnectionResult.connect(slot, Engine::SignalSlot::queuedConnection);
			}

		protected:
			virtual void removeSlaveStream() override;
			virtual void removeMasterStream(Engine::Serialize::BufferedInOutStream *stream) override;

			BoostIPCStream &addMasterStream(BoostIPCStream &&stream, bool sendState = true);

			bool connectImpl(int timeout);

		private:
			BoostIPCServer *mServer;
			SharedMemory *mMemory;

			bool mIsServer;			

			std::map<Engine::Serialize::ParticipantId, BoostIPCStream> mStreams;
			BoostIPCStream *mSlaveStream;

			//static constexpr UINT sMessageSignature = 1048;

			//static int sManagerCount;

			Engine::SignalSlot::Signal<bool> mConnectionResult;

		};

	}
}

