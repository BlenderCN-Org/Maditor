#include "maditormodellib.h"
#include "Inspector.h"

namespace Maditor {
	namespace Model {

		Inspector::Inspector() :
			TreeUnit(1),
			mGlobalWrapper(this)
		{
		}

		TreeItem * Inspector::child(int i)
		{
			assert(i == 0);
			return &mGlobalWrapper;
		}

		int Inspector::childCount() const
		{
			return 1;
		}

		void Inspector::requestUpdate(ScopeWrapper *wrapper)
		{
			mWrappers[wrapper->ptr()] = wrapper;
			mRequestUpdate(wrapper->ptr(), {});
		}

		void Inspector::sendUpdateImpl(Engine::InvScopePtr ptr, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes) {
			mWrappers.at(ptr)->update(attributes.data());
		}

	}
}