#pragma once

#include "Model\Application\TreeUnit.h"

namespace Maditor {
	namespace Model {

		class ValueWrapper {
		public:
			ValueWrapper(const std::string &name, const Engine::ValueType &value);

			std::string toString() const;
			const std::string &name() const;

			void operator=(const Engine::ValueType &value);

		private:
			std::string mName;
			Engine::ValueType mValue;
		};

		class ValueItem : public TreeItem {
		public:
			ValueItem(ScopeWrapperItem *parent, ValueWrapper *val);

			virtual TreeItem *child(int) override { throw 0; };
			virtual QVariant data(int col) const override;
			virtual TreeItem *parentItem() const override;

		private:
			ValueWrapper *mValue;
			ScopeWrapperItem *mParent;
		};

		class ScopeWrapper {
		public:
			ScopeWrapper(Inspector *inspector, Engine::InvScopePtr ptr, const std::string &name);

			
			void clear();

			Engine::InvScopePtr ptr() const;

			int childCount() const;
			int valueCount() const;

			void update(const std::map<std::string, Engine::ValueType> &data);

			void addItem(ScopeWrapperItem *item);
			void removeItem(ScopeWrapperItem *item);

			const std::string &name() const;

			Inspector *inspector();

		private:
			Engine::InvScopePtr mPtr;
			std::string mName;

			std::map<Engine::InvScopePtr, std::shared_ptr<ScopeWrapper>> mChildren;
			std::map<std::string, ValueWrapper> mValues;

			std::list<ScopeWrapperItem*> mItems;

			Inspector *mInspector;

		};

		class ScopeWrapperItem : public TreeItem {
		public:
			ScopeWrapperItem(Inspector *parent, const std::shared_ptr<ScopeWrapper> &scope);
			ScopeWrapperItem(ScopeWrapperItem *parent, const std::shared_ptr<ScopeWrapper> &scope);
			virtual ~ScopeWrapperItem();

			void clearValues();
			void clear();

			virtual int childCount() const override;
			virtual TreeItem *child(int i) override;
			virtual QVariant data(int col) const override;		
			virtual TreeItem *parentItem() const override;

			void addChild(const std::shared_ptr<ScopeWrapper> &scope);
			void removeChild(int i);
			void addValue(ValueWrapper *value);
			void removeValue(int i);

			QModelIndex getIndex();

		private:
			std::shared_ptr<ScopeWrapper> mScope;

			ScopeWrapperItem *mScopeParent;
			TreeItem *mParent;

			std::list<ScopeWrapperItem> mChildren;
			std::list<ValueItem> mValues;
			

		};
	}
}