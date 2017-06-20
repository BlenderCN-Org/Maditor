#pragma once

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Launcher {
		class Inspector : public Engine::Serialize::SerializableUnit<Inspector> {
		protected:
			void requestUpdateImpl(Engine::InvScopePtr ptr);

			bool isValid(Engine::InvScopePtr ptr);

			void sendUpdateImpl(Engine::InvScopePtr ptr, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes) {}

		private:
			std::map<Engine::InvScopePtr, Engine::InvScopePtr> mParents;

			Engine::Serialize::Action<decltype(&Inspector::requestUpdateImpl), &Inspector::requestUpdateImpl, Engine::Serialize::ActionPolicy::request> mRequestUpdate;
			Engine::Serialize::Action<decltype(&Inspector::sendUpdateImpl), &Inspector::sendUpdateImpl, Engine::Serialize::ActionPolicy::notification> mSendUpdate;
		};
	}
}