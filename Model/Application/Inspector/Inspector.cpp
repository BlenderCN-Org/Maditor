#include "maditormodellib.h"
#include "Inspector.h"

namespace Maditor {
	namespace Model {

		Inspector::Inspector(Engine::Serialize::TopLevelSerializableUnitBase *topLevel) :
			TreeUnit(topLevel, 2),
			mGlobalWrapper(std::make_shared<ScopeWrapper>(this, nullptr, "Global")),
			mGlobalWrapperItem(this, mGlobalWrapper),
			mPending(false),
			mTimer(0)
		{
			reset();
		}

		void Inspector::start()
		{
			mTimer = startTimer(100);
		}

		void Inspector::reset()
		{
			beginResetModel();
			mGlobalWrapper->clear();
			mWrappers.clear();
			mWrappers[nullptr] = mGlobalWrapper;
			mIt = mWrappers.begin();
			if (mTimer) {
				killTimer(mTimer);
				mTimer = 0;
				mPending = false;
			}
			endResetModel();
		}

		TreeItem * Inspector::child(int i)
		{
			assert(i == 0);
			return &mGlobalWrapperItem;
		}

		int Inspector::childCount() const
		{
			return 1;
		}

		void Inspector::timerEvent(QTimerEvent * e)
		{
			if (mPending)
				return;
			mPending = true;
			mRequestUpdate(mIt->first, {});
			++mIt;
			if (mIt == mWrappers.end())
				mIt = mWrappers.begin();			
		}

		std::shared_ptr<ScopeWrapper> Inspector::getScope(Engine::InvScopePtr ptr, const std::string &name)
		{
			auto it = mWrappers.find(ptr);
			if (it != mWrappers.end()) {
				if (std::shared_ptr<ScopeWrapper> ptr = it->second.lock()) {
					return ptr;
				}
				if (mIt == it) {
					++mIt;
					if (mIt == mWrappers.end())
						mIt = mWrappers.begin();
				}
				mWrappers.erase(it);
			}
			
			std::shared_ptr<ScopeWrapper> scope = std::make_shared<ScopeWrapper>(this, ptr, name);
			mWrappers.try_emplace(ptr, scope);
			return scope;
		}

		void Inspector::sendUpdateImpl(Engine::InvScopePtr ptr, bool exists, const Engine::Serialize::SerializableMap<std::string, Engine::ValueType> &attributes) {
			mPending = false;
			auto it = mWrappers.find(ptr);
			assert(it != mWrappers.end());
			if (exists) {
				if (std::shared_ptr<ScopeWrapper> p = it->second.lock()) {
					p->update(attributes.data());
				}
				else {
					mWrappers.erase(it);
				}
			}
			else {
				mWrappers.erase(it);
			}
		}



	}
}