#include <maditorlauncherlib.h>
#include "Inspector.h"

#include "Scripting\Types\globalscopebase.h"
#include "App/application.h"

#include "Scripting/Parsing/scriptparser.h"

extern "C" {
#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */
}

API_IMPL(Maditor::Launcher::InspectorThreadInstance);

namespace Maditor {
	namespace Launcher {

		std::mutex InspectorThreadInstance::sMappingsMutex;
		std::map<Engine::App::Application*, InspectorThreadInstance *> InspectorThreadInstance::sMappings;

		const luaL_Reg Inspector::sMarkMetafunctions[] =
		{
			{ "__gc", &lua_gc },
			//{ "__pairs", &lua_pairs },
			//{"__newindex", &lua_newindex}
			{ NULL, NULL }
		};


		int Inspector::lua_gc(lua_State *state) {
			bool **ptr = static_cast<bool**>(lua_touserdata(state, -1));
			**ptr = false;
			lua_pop(state, 1);
			return 0;
		}


		InspectorThreadInstance::InspectorThreadInstance() :
			mUpdate(this)
		{
		}

		lua_State * InspectorThreadInstance::state()
		{
			return mState;
		}

		bool InspectorThreadInstance::init()
		{
			if (Engine::Scripting::GlobalAPIComponent<InspectorThreadInstance>::init()) {
				mState = Engine::App::Application::getSingleton().lua_state();
				sMappingsMutex.lock();
				sMappings[&Engine::App::Application::getSingleton()] = this;
				sMappingsMutex.unlock();
				return true;
			}
			else {
				return false;
			}			
		}

		void InspectorThreadInstance::finalize()
		{
			sMappingsMutex.lock();
			sMappings.erase(&Engine::App::Application::getSingleton());
			sMappingsMutex.unlock();
			Engine::Scripting::GlobalAPIComponent<InspectorThreadInstance>::finalize();
		}

		void InspectorThreadInstance::getUpdate(Engine::InvScopePtr ptr, Inspector * inspector)
		{
			mUpdate.queue(ptr, inspector);
		}

		void InspectorThreadInstance::update(Engine::InvScopePtr ptr, Inspector * inspector)
		{
			inspector->getUpdate(ptr, this);
		}

		Inspector::Inspector()
		{
			 
		}

		void Inspector::getUpdate(Engine::InvScopePtr ptr, InspectorThreadInstance *thread)
		{
			Engine::Scripting::ScopeBase *scope;
			if (isValid(ptr)) {
				scope = validate(ptr);
			}
			else {
				mItemRemoved->queue(ptr);
				return;
			}

			Engine::Serialize::SerializableMap<std::string, Engine::ValueType> attributes;
			for (std::unique_ptr<Engine::Scripting::KeyValueIterator> it = scope->iterator(); !it->ended(); ++(*it)) {
				std::pair<bool, Engine::ValueType> value = it->value();
				if (value.first) {
					if (value.second.isScope()) {
						attributes.try_emplace(it->key(), Engine::ValueType(Engine::InvScopePtr(value.second.asScope())));
						mItemsMutex.lock();
						auto it = mItems.find(value.second.asScope());
						if (it == mItems.end()) {
							markItem(value.second.asScope(), thread);
						}
						mItemsMutex.unlock();
					}
					else
						attributes.try_emplace(it->key(), value.second);
				}
			}
			mItemUpdate->queue(ptr, attributes);
			
		}

		void Inspector::init() {
			lua_State *state = Engine::Scripting::GlobalScopeBase::getSingleton().lua_state();

			luaL_newmetatable(state, "Maditor.InspectorMarkMetatable");

			luaL_setfuncs(state, sMarkMetafunctions, 0);

			lua_pop(state, 1);

			mItemRemoved = std::make_unique<Engine::SignalSlot::Slot<decltype(&Inspector::itemRemoved), &Inspector::itemRemoved>>(this);
			mItemUpdate = std::make_unique<Engine::SignalSlot::Slot<decltype(&Inspector::itemUpdate), &Inspector::itemUpdate>>(this);
		}

		void Inspector::itemRemoved(Engine::InvScopePtr ptr)
		{
			mItemsMutex.lock();
			mItems.erase(ptr);
			mItemsMutex.unlock();
			mSendItemRemoved(ptr, {});
		}

		void Inspector::itemUpdate(Engine::InvScopePtr ptr, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType>& attributes)
		{
			mSendUpdate(ptr, attributes, {});
		}

		void Inspector::requestUpdateImpl(Engine::InvScopePtr ptr)
		{			
			if (ptr) {
				mItemsMutex.lock();
				auto it = mItems.find(ptr);
				mItemsMutex.unlock();
				if (it == mItems.end()) {
					mSendItemRemoved(ptr, {});
				}
				else {
					InspectorThreadInstance *thread = std::get<0>(it->second);
					if (thread) {
						thread->getUpdate(ptr, this);
					}
					else {
						getUpdate(ptr, nullptr);
					}
				}
			}
			else {
				getUpdate(ptr, nullptr);
			}
			
		}

		bool Inspector::isValid(Engine::InvScopePtr ptr)
		{
			if (!ptr) {
				return true;
			}
			else {
				mItemsMutex.lock();
				auto it = mItems.find(ptr);
				bool valid = std::get<1>(it->second);
				mItemsMutex.unlock();
				return valid;
			}
		}

		void Inspector::markItem(Engine::Scripting::ScopeBase * ptr, InspectorThreadInstance * thread)
		{
			std::tuple<InspectorThreadInstance*, bool> &p = mItems[ptr] = std::make_tuple(thread, true);
			
			ptr->push();

			lua_State *state = thread ? thread->state() : Engine::Scripting::GlobalScopeBase::getSingleton().lua_state();

			bool **bPtr = static_cast<bool**>(lua_newuserdata(state, sizeof(bool*)));
			*bPtr = &std::get<1>(p);

			luaL_getmetatable(state, "Maditor.InspectorMarkMetatable");

			lua_setmetatable(state, -2);

			lua_setfield(state, -2, "__InspectorMark__");

			lua_pop(state, 1);

		}

		Engine::Scripting::ScopeBase * Inspector::validate(Engine::InvScopePtr ptr)
		{
			if (ptr)
				return ptr.validate();
			else
				return &Engine::Scripting::GlobalScopeBase::getSingleton();
		}

	}
}