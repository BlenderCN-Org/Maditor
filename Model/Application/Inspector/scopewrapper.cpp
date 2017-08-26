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

		void ValueWrapper::operator=(const Engine::ValueType & value)
		{
			mValue = value;
		}

		const std::string &ValueWrapper::name() const
		{
			return mName;
		}

		ValueItem::ValueItem(ScopeWrapperItem * parent, ValueWrapper *val) :
			mParent(parent),
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

		TreeItem * ValueItem::parentItem() const
		{
			return mParent;
		}

		ScopeWrapperItem::ScopeWrapperItem(Inspector * parent, const std::shared_ptr<ScopeWrapper> &scope) :
			mParent(parent),	
			mScopeParent(nullptr),
			mScope(scope)
		{
			scope->addItem(this);
		}

		ScopeWrapperItem::ScopeWrapperItem(ScopeWrapperItem * parent, const std::shared_ptr<ScopeWrapper> &scope) :
			mParent(parent),
			mScopeParent(parent),
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
			std::set<Engine::InvScopePtr> ptrs;
			std::set<std::string> attributes;
			for (const std::pair<const std::string, Engine::ValueType> &p : data) {
				if (p.second.is<Engine::InvScopePtr>()) {
					std::shared_ptr<ScopeWrapper> scope = mInspector->getScope(p.second.as<Engine::InvScopePtr>(), p.first);
					if (mChildren.try_emplace(p.second.as<Engine::InvScopePtr>(), scope).second) {
						for (ScopeWrapperItem *item : mItems) {
							item->addChild(scope);
						}
					}
					ptrs.insert(p.second.as<Engine::InvScopePtr>());
				}
				else {					
					std::pair<std::map<std::string, ValueWrapper>::iterator, bool> result = mValues.try_emplace(p.first, p.first, p.second);
					if (!result.second) {
						result.first->second = p.second;
					}else{
						for (ScopeWrapperItem *item : mItems) {
							item->addValue(&result.first->second);
						}
					}
					attributes.insert(p.first);
				}
			}
			int i = 0;
			for (auto it = mChildren.begin(); it != mChildren.end(); ){
				if (ptrs.find(it->first) == ptrs.end()) {
					for (ScopeWrapperItem *item : mItems) {
						item->removeChild(i);
					}
					it = mChildren.erase(it);
				}
				else {
					++it;
					++i;
				}
			}
			i = 0;
			for (auto it = mValues.begin(); it != mValues.end(); ) {
				if (attributes.find(it->first) == attributes.end()) {
					for (ScopeWrapperItem *item : mItems) {
						item->removeValue(i);
					}
					it = mValues.erase(it);
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

		void ScopeWrapperItem::removeChild(int i)
		{
			mScope->inspector()->beginRemoveRows(getIndex(), i, i);
			auto it = mChildren.begin();
			std::advance(it, i);
			mChildren.erase(it);
			mScope->inspector()->endRemoveRows();
		}

		void ScopeWrapperItem::addValue(ValueWrapper * value)
		{
			mScope->inspector()->beginInsertRows(getIndex(), mChildren.size() + mValues.size(), mChildren.size() + mValues.size());
			mValues.emplace_back(this, value);
			mScope->inspector()->endInsertRows();
		}

		void ScopeWrapperItem::removeValue(int i)
		{
			mScope->inspector()->beginRemoveRows(getIndex(), i + mChildren.size(), i + mChildren.size());
			auto it = mValues.begin();
			std::advance(it, i);
			mValues.erase(it);
			mScope->inspector()->endRemoveRows();
		}

		TreeItem *ScopeWrapperItem::parentItem() const {
			return mParent;
		}

		QModelIndex ScopeWrapperItem::getIndex() {
			return mScope->inspector()->index(parentIndex(), 0, mScopeParent ? mScopeParent->getIndex() : QModelIndex());
		}

	}
}