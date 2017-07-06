#pragma once

#include "Serialize\Container\map.h"

#include "scopewrapper.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Model {
		class Inspector : public TreeUnit<Inspector> {
			Q_OBJECT
		public:
			Inspector();

			void start();
			void reset();

			virtual TreeItem *child(int i) override;
			virtual int childCount() const override;

			void requestUpdate(const std::shared_ptr<ScopeWrapper> &wrapper);

			void sendUpdateImpl(Engine::InvScopePtr ptr, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes);

			std::shared_ptr<ScopeWrapper> getScope(Engine::InvScopePtr ptr, const std::string &name);

		protected:
			void requestUpdateImpl(Engine::InvScopePtr ptr) {}

			virtual void timerEvent(QTimerEvent *e) override;

		private:
			std::shared_ptr<ScopeWrapper> mGlobalWrapper;
			ScopeWrapperItem mGlobalWrapperItem;

			std::map<Engine::InvScopePtr, std::weak_ptr<ScopeWrapper>> mWrappers;
			std::map<Engine::InvScopePtr, std::weak_ptr<ScopeWrapper>>::iterator mIt;

			Engine::Serialize::Action<decltype(&Inspector::requestUpdateImpl), &Inspector::requestUpdateImpl, Engine::Serialize::ActionPolicy::request> mRequestUpdate;
			Engine::Serialize::Action<decltype(&Inspector::sendUpdateImpl), &Inspector::sendUpdateImpl, Engine::Serialize::ActionPolicy::notification> mSendUpdate;

			bool mPending;
			int mTimer;
		};
	}
}