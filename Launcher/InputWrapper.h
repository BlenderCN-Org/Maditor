#pragma once



#include "Input\InputHandler.h"


namespace Maditor {

	

	namespace Launcher {
		
		class InputWrapper : public Engine::Input::InputHandler {

		public:
			InputWrapper(Shared::InputShared &shared);


			virtual void update() override;

			void setSystem(Engine::GUI::GUISystem *gui);


		private:
			Engine::GUI::GUISystem *mSystem;

			Shared::InputShared &mShared;

		};

	}
}