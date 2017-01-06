#include "maditormodellib.h"

#include "InputWrapper.h"

#include "Shared/InputInfo.h"

namespace Maditor {
	namespace Model {

			InputWrapper::InputWrapper(Shared::InputShared &shared) :
				mShared(shared)
			{
				resetAccumulativeMouseMove();
			}

			void InputWrapper::keyPressEvent(QKeyEvent * ev)
			{
				char text = ev->text().isEmpty() ? 0 : ev->text().at(0).toLatin1();
				mShared.mKeyQueue.emplace(Shared::PRESS, Engine::GUI::KeyEventArgs{ (Engine::GUI::Key)ev->nativeScanCode(), text });
			}

			void InputWrapper::keyReleaseEvent(QKeyEvent * ev)
			{
				char text = ev->text().isEmpty() ? 0 : ev->text().at(0).toLatin1();			
				mShared.mKeyQueue.emplace(Shared::RELEASE, Engine::GUI::KeyEventArgs{ (Engine::GUI::Key)ev->nativeScanCode(), text });
			}

			void InputWrapper::mouseMoveEvent(QMouseEvent * ev)
			{
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mShared.mMutex);
				mShared.mAccumulativeMouseMove.moveDelta.x += ev->x() - mShared.mAccumulativeMouseMove.position.x;
				mShared.mAccumulativeMouseMove.moveDelta.y += ev->y() - mShared.mAccumulativeMouseMove.position.y;
				mShared.mAccumulativeMouseMove.position.x = ev->x();
				mShared.mAccumulativeMouseMove.position.y = ev->y();
				mShared.mMove = true;
			}

			void InputWrapper::wheelEvent(QWheelEvent * ev)
			{
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mShared.mMutex);
				mShared.mAccumulativeMouseMove.scrollWheel += ev->angleDelta().y() / 120.0f;
				mShared.mMove = true;
			}

			void InputWrapper::mousePressEvent(QMouseEvent * ev)
			{
				mShared.mMouseQueue.emplace(Shared::PRESS, Engine::GUI::MouseEventArgs{ { (float)ev->x(), (float)ev->y() }, convertMouseButton(ev->button()) });
			}

			void InputWrapper::mouseReleaseEvent(QMouseEvent * ev)
			{
				mShared.mMouseQueue.emplace(Shared::RELEASE, Engine::GUI::MouseEventArgs{ { (float)ev->x(), (float)ev->y() }, convertMouseButton(ev->button()) });
			}

			void InputWrapper::resetAccumulativeMouseMove()
			{
				boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(mShared.mMutex);
				mShared.mMove = false;
				mShared.mAccumulativeMouseMove.moveDelta = { 0,0 };
				mShared.mAccumulativeMouseMove.scrollWheel = 0;
			}

			Engine::GUI::MouseButton::MouseButton InputWrapper::convertMouseButton(Qt::MouseButton id)
			{
				switch (id) {
				case Qt::LeftButton:
					return Engine::GUI::MouseButton::LEFT_BUTTON;
					break;
				case Qt::RightButton:
					return Engine::GUI::MouseButton::RIGHT_BUTTON;
					break;
				case Qt::MiddleButton:
					return Engine::GUI::MouseButton::MIDDLE_BUTTON;
					break;
				default:
					throw 0;
				}
			}

		
	}
}