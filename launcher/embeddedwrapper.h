#pragma once

#include "ApplicationWrapper.h"

#include "Shared/SharedMemory.h"
#include "Shared/IPCManager/boostIPCmanager.h"

namespace Maditor
{
	namespace Launcher
	{
		
		class EmbeddedWrapper : public ApplicationWrapper
		{
		public:
			EmbeddedWrapper(size_t id);
			~EmbeddedWrapper();

		protected:
			int acceptConnection() override;
			Engine::Serialize::SerializeManager* mgr() override;

			
		private:
			Shared::SharedMemory mMemory;
			Shared::BoostIPCManager mNetwork;

#ifdef MADGINE_CLIENT_BUILD
		protected:
			Engine::Input::InputHandler* input() override;
		private:
			InputWrapper *mInput;
#endif
		};

		
	}
}