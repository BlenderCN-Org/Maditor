#include <maditorlauncherlib.h>
#include "Inspector.h"

#include "Scripting\Types\GlobalScope.h"

#include <iostream>

namespace Maditor {
	namespace Launcher {

		void Inspector::requestUpdateImpl(Engine::InvScopePtr ptr)
		{
			Engine::Scripting::ScopeBase *scope;
			if (!ptr) {
				scope = &Engine::Scripting::GlobalScope::getSingleton();
			}
			else {
				if (isValid(ptr)) {
					scope = ptr.validate();
				}
				else {
					mParents.erase(ptr);
					return;
				}
			}
			Engine::Serialize::SerializableMap<std::string, Engine::ValueType> attributes;
			Engine::Scripting::KeyValueIterator *it;
			for (it = scope->iterator(); !it->ended(); ++(*it)) {
				std::pair<bool, Engine::ValueType> value = it->value();
				if (value.first) {
					if (value.second.isScope())
						attributes.try_emplace(it->key(), Engine::ValueType(Engine::InvScopePtr(value.second.asScope())));
					else
						attributes.try_emplace(it->key(), value.second);
				}
			}
			delete it;
			mSendUpdate(ptr, attributes, {});
		}

		bool Inspector::isValid(Engine::InvScopePtr ptr)
		{
			if (!ptr) {
				return true;
			}
			else {
				auto it = mParents.find(ptr);
				if (it == mParents.end()) {
					return false;
				}
				else {
					return isValid(it->second);
				}
			}
		}

	}
}