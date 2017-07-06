#include "maditormodellib.h"
#include "scopewrapper.h"

#include "Inspector.h"

namespace Maditor {
	namespace Model {

		ValueWrapper::ValueWrapper(const std::string & name, const Engine::ValueType & value) :
			mName(name),
			mValue(value)
		{
		}

		std::string ValueWrapper::toString() const
		{
			return mValue.toString();
		}

		const std::string &ValueWrapper::name() const
		{
			return mName;
		}

		ValueItem::ValueItem(ScopeWrapperItem * parent, ValueWrapper *val) :
			TreeUnitItem(parent),
			mValue(val)
		{

		}

		QVariant ValueItem::data(int col) const
		{
			switch (col) {
			case 0:
				return QString::fromStdString(mValue->name());
			case 1:
				return QString::fromStdString(mValue->toString());
			}
			return QVariant();
		}

		ScopeWrapperItem::ScopeWrapperItem(Inspector * parent, const std::shared_ptr<ScopeWrapper> &scope) :
			TreeUnitItem(static_cast<TreeUnitItemBase*>(parent)),			
			mScope(scope)
		{
			scope->addItem(this);
		}

		ScopeWrapperItem::ScopeWrapperItem(ScopeWrapperItem * parent, const std::shared_ptr<ScopeWrapper> &scope) :
			TreeUnitItem(parent),
			mScope(scope)
		{
			scope->addItem(this);
		}

		ScopeWrapperItem::~ScopeWrapperItem()
		{
			mScope->removeItem(this);
		}

		int ScopeWrapper::childCount() const
		{
			return mChildren.size();
		}

		int ScopeWrapper::valueCount() const
		{
			return mValues.size();
		}

		void ScopeWrapper::clear()
		{
			mValues.clear();
			mChildren.clear();
			for (ScopeWrapperItem *item : mItems)
				item->clear();
		}

		Engine::InvScopePtr ScopeWrapper::ptr() const
		{
			return mPtr;
		}

		int ScopeWrapperItem::childCount() const
		{
			return mScope->childCount() + mScope->valueCount();
		}

		void ScopeWrapper::addItem(ScopeWrapperItem * item)
		{
			mItems.push_back(item);
		}

		void ScopeWrapper::removeItem(ScopeWrapperItem * item)
		{
			mItems.remove(item);
		}

		TreeItem * ScopeWrapperItem::child(int i)
		{
			if (i < mChildren.size()) {
				auto it = mChildren.begin();
				std::advance(it, i);
				return &*it;
			}
			else {
				auto it = mValues.begin();
				std::advance(it, i - mChildren.size());
				return &*it;
			}
		}

		QVariant ScopeWrapperItem::data(int col) const
		{
			switch (col) {
			case 0:
				return QString::fromStdString(mScope->name());
			}
			return QVariant();
		}

		const std::string &ScopeWrapper::name() const
		{
			return mName;
		}

		void ScopeWrapperItem::clearValues()
		{
			mValues.clear();
		}

		void ScopeWrapperItem::clear()
		{
			mValues.clear();
			mChildren.clear();
		}

		ScopeWrapper::ScopeWrapper(Inspector * inspector, Engine::InvScopePtr ptr, const std::string & name) :
			mInspector(inspector),
			mPtr(ptr),
			mName(name)
		{
		}

		void ScopeWrapper::update(const std::map<std::string, Engine::ValueType>& data)
		{
			mValues.clear();
			for (ScopeWrapperItem *item : mItems)
				item->clearValues();
			std::set<Engine::InvScopePtr> ptrs;
			for (const std::pair<const std::string, Engine::ValueType> &p : data) {
				if (p.second.isInvPtr()) {
					std::shared_ptr<ScopeWrapper> scope = mInspector->getScope(p.second.asInvPtr(), p.first);
					if (mChildren.try_emplace(p.second.asInvPtr(), scope).second) {
						for (ScopeWrapperItem *item : mItems) {
							item->addChild(scope);
						}
					}
					ptrs.insert(p.second.asInvPtr());
				}
				else {
					mValues.emplace_back(p.first, p.second);
				}
			}
			int i = 0;
			for (auto it = mChildren.begin(); it != mChildren.end(); ){
				if (ptrs.find(it->first) == ptrs.end()) {					
					//mInspector->beginRemoveRows(getIndex(), i, i);
					it = mChildren.erase(it);
					//mInspector->endRemoveRows();
				}
				else {
					++it;
					++i;
				}
			}
		}

		Inspector * ScopeWrapper::inspector()
		{
			return mInspector;
		}

		void ScopeWrapperItem::addChild(const std::shared_ptr<ScopeWrapper>& scope)
		{
			mScope->inspector()->beginInsertRows(getIndex(), mChildren.size(), mChildren.size());
			mChildren.emplace_back(this, scope);
			mScope->inspector()->endInsertRows();
		}

	}
}