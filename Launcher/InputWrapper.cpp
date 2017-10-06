#include "maditorlauncherlib.h"

#ifdef MADGINE_CLIENT_BUILD

#include "InputWrapper.h"

#include "Shared\InputInfo.h"

#include "GUI\GUISystem.h"

namespace Maditor {

	//

	namespace Launcher {

		InputWrapper::InputWrapper(Shared::InputShared &shared) :
			mShared(shared)
		{

		}

		void InputWrapper::update()
		{

			while (!mShared.mMouseQueue.empty()) {
				const std::pair<Shared::InputEventType, Shared::MouseEventArgs> &evt = mShared.mMouseQueue.front();
				if (evt.first == Shared::PRESS) {
					mSystem->injectMousePress({ evt.second.position, convertButton(evt.second.button) });
				}
				else {
					mSystem->injectMouseRelease({ evt.second.position, convertButton(evt.second.button) });
				}
				mShared.mMouseQueue.pop();
			}

			while (!mShared.mKeyQueue.empty()) {
				const std::pair<Shared::InputEventType, Shared::KeyEventArgs> &evt = mShared.mKeyQueue.front();
				if (evt.first == Shared::PRESS) {
					mSystem->injectKeyPress({ (Engine::GUI::Key)evt.second.key, evt.second.text });
				}
				else {
					mSystem->injectKeyRelease({ (Engine::GUI::Key)evt.second.key, evt.second.text});
				}
				mShared.mKeyQueue.pop();
			}


			boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mShared.mMutex);
			if (mShared.mMove) {
				mSystem->injectMouseMove({ mShared.mAccumulativeMouseMove.position, mShared.mAccumulativeMouseMove.moveDelta, mShared.mAccumulativeMouseMove.scrollWheel });
				mShared.mMove = false;
				mShared.mAccumulativeMouseMove.moveDelta = { 0,0 };
				mShared.mAccumulativeMouseMove.scrollWheel = 0;
			}
		}

		void InputWrapper::setSystem(Engine::GUI::GUISystem * gui)
		{
			mSystem = gui;
		}

		Engine::GUI::MouseButton::MouseButton InputWrapper::convertButton(Shared::MouseButton button)
		{
			return Engine::GUI::MouseButton::MouseButton(button);
		}

	}
}

#endif