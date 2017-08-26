#pragma once

#include "Serialize\Container\action.h"
#include "serialize/container/map.h"
#include "Scripting/Types/globalapicomponent.h"

#include "signalslot/slot.h"

namespace Maditor {
	namespace Launcher {

		class InspectorThreadInstance : public Engine::Scripting::GlobalAPIComponent<InspectorThreadInstance> {
		public:
			InspectorThreadInstance();

			void getUpdate(Engine::InvScopePtr ptr, Inspector *inspector);

			lua_State *state();

			virtual bool init() override;
			virtual void finalize() override;

			static InspectorThreadInstance *getInstance(lua_State *thread);
			Engine::Scripting::GlobalScopeBase *globalScope();

		protected:
			void update(Engine::InvScopePtr ptr, Inspector *inspector);

		private:
			Engine::SignalSlot::Slot<decltype(&InspectorThreadInstance::update), &InspectorThreadInstance::update> mUpdate;

			static std::mutex sMappingsMutex;
			static std::map<lua_State *, InspectorThreadInstance*> sMappings;

			lua_State *mState;

			Engine::Scripting::GlobalScopeBase *mGlobalScope;
		};

		class Inspector : public Engine::Serialize::SerializableUnit<Inspector> {
		public:
			Inspector(Engine::Serialize::TopLevelSerializableUnitBase *topLevel);

			void init();
			void getUpdate(Engine::InvScopePtr ptr, InspectorThreadInstance *thread);
		
		protected:
			void requestUpdateImpl(Engine::InvScopePtr ptr);
			void sendUpdateImpl(Engine::InvScopePtr ptr, bool exists, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes) {}

			void itemRemoved(Engine::InvScopePtr ptr);
			void itemUpdate(Engine::InvScopePtr ptr, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes);

		protected:
			Engine::Scripting::ScopeBase *validate(Engine::InvScopePtr ptr);
			bool isValid(Engine::InvScopePtr ptr);
			void markItem(Engine::Scripting::ScopeBase *ptr, InspectorThreadInstance *thread);

		private:
			std::mutex mItemsMutex;
			std::map<Engine::InvScopePtr, std::tuple<InspectorThreadInstance*, bool>> mItems;

			Engine::Serialize::Action<decltype(&Inspector::requestUpdateImpl), &Inspector::requestUpdateImpl, Engine::Serialize::ActionPolicy::request> mRequestUpdate;
			Engine::Serialize::Action<decltype(&Inspector::sendUpdateImpl), &Inspector::sendUpdateImpl, Engine::Serialize::ActionPolicy::notification> mSendUpdate;
			std::unique_ptr<Engine::SignalSlot::Slot<decltype(&Inspector::itemRemoved), &Inspector::itemRemoved>> mItemRemoved;
			std::unique_ptr<Engine::SignalSlot::Slot<decltype(&Inspector::itemUpdate), &Inspector::itemUpdate>> mItemUpdate;

			static const luaL_Reg sMarkMetafunctions[];
			static int lua_scopeGc(lua_State *);
		};
	}
}