#pragma once

#include "Serialize\Container\map.h"

#include "scopewrapper.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Model {
		class Inspector : public TreeUnit<Inspector> {
		public:
			Inspector();

			virtual TreeItem *child(int i) override;
			virtual int childCount() const override;

			void requestUpdate(ScopeWrapper *wrapper);

			void sendUpdateImpl(Engine::InvScopePtr ptr, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes);

		protected:
			void requestUpdateImpl(Engine::InvScopePtr ptr) {}

		private:
			ScopeWrapper mGlobalWrapper;

			std::map<Engine::InvScopePtr, ScopeWrapper*> mWrappers;

			Engine::Serialize::Action<decltype(&Inspector::requestUpdateImpl), &Inspector::requestUpdateImpl, Engine::Serialize::ActionPolicy::request> mRequestUpdate;
			Engine::Serialize::Action<decltype(&Inspector::sendUpdateImpl), &Inspector::sendUpdateImpl, Engine::Serialize::ActionPolicy::notification> mSendUpdate;
		};
	}
}