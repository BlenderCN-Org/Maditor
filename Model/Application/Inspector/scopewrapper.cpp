#include "maditormodellib.h"
#include "scopewrapper.h"

#include "Inspector.h"

namespace Maditor {
	namespace Model {

		ValueItem::ValueItem(ScopeWrapper * parent, const std::string & name, const Engine::ValueType & value) :
			TreeUnitItem(parent),
			mName(name),
			mValue(value)
		{
		}

		QVariant ValueItem::data(int col) const
		{
			switch (col) {
			case 0:
				return QString::fromStdString(mName);
			case 1:
				return QString::fromStdString(mValue.toString());
			}
			return QVariant();
		}

		ScopeWrapper::ScopeWrapper(Inspector * parent) :
			TreeUnitItem(static_cast<TreeUnitItemBase*>(parent)),
			mName("Global"),
			mInspector(parent),
			mPtr(nullptr)
		{
		}

		ScopeWrapper::ScopeWrapper(ScopeWrapper * parent, Engine::InvScopePtr ptr, const std::string & name) :
			TreeUnitItem(parent),
			mPtr(ptr),
			mName(name),
			mInspector(parent->mInspector)
		{
		}

		Engine::InvScopePtr ScopeWrapper::ptr() const
		{
			return mPtr;
		}

		int ScopeWrapper::childCount() const
		{
			mInspector->requestUpdate(const_cast<ScopeWrapper*>(this));
			return mChildren.size() + mValues.size();
		}

		TreeItem * ScopeWrapper::child(int i)
		{
			if (i < mChildren.size()) {
				auto it = mChildren.begin();
				std::advance(it, i);
				return &it->second;
			}
			else {
				auto it = mValues.begin();
				std::advance(it, i - mChildren.size());
				return &*it;
			}
		}

		QVariant ScopeWrapper::data(int col) const
		{
			switch (col) {
			case 0:
				return QString::fromStdString(mName);
			}
			return QVariant();
		}

		void ScopeWrapper::update(const std::map<std::string, Engine::ValueType>& data)
		{
			mValues.clear();
			std::set<Engine::InvScopePtr> ptrs;
			for (const std::pair<const std::string, Engine::ValueType> &p : data) {
				if (p.second.isInvPtr()) {
					mChildren.try_emplace(p.second.asInvPtr(), this, p.second.asInvPtr(), p.first);
					ptrs.insert(p.second.asInvPtr());
				}
				else {
					mValues.emplace_back(this, p.first, p.second);
				}
			}
			for (auto it = mChildren.begin(); it != mChildren.end(); ){
				if (ptrs.find(it->first) == ptrs.end())
					it = mChildren.erase(it);
				else
					++it;
			}
		}

	}
}