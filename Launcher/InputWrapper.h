#pragma once



#include "Input\InputHandler.h"


namespace Maditor {
	namespace Launcher {
		
		class InputWrapper : public Engine::Input::InputHandler {

		public:
			InputWrapper(Shared::InputShared &shared);


			virtual void update() override;

			void setSystem(Engine::GUI::GUISystem *gui);

		protected:
			static Engine::GUI::MouseButton::MouseButton convertButton(Shared::MouseButton button);

		private:
			Engine::GUI::GUISystem *mSystem;

			Shared::InputShared &mShared;

		};

	}
}