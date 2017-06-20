#pragma once

#include "Model\Application\TreeUnit.h"

namespace Maditor {
	namespace Model {
		class ValueItem : public TreeUnitItem<ValueItem> {
		public:
			ValueItem(ScopeWrapper *parent, const std::string &name, const Engine::ValueType &value);

			virtual TreeItem *child(int) override { throw 0; };
			virtual QVariant data(int col) const override;

		private:
			std::string mName;
			Engine::ValueType mValue;

		};

		class ScopeWrapper : public TreeUnitItem<ScopeWrapper> {
		public:
			ScopeWrapper(Inspector *parent);
			ScopeWrapper(ScopeWrapper *parent, Engine::InvScopePtr ptr, const std::string &name);

			virtual int childCount() const override;
			virtual TreeItem *child(int i) override;
			virtual QVariant data(int col) const override;

			Engine::InvScopePtr ptr() const;

			void update(const std::map<std::string, Engine::ValueType> &data);

		private:
			Engine::InvScopePtr mPtr;
			std::string mName;

			Inspector *mInspector;

			std::map<Engine::InvScopePtr, ScopeWrapper> mChildren;
			std::list<ValueItem> mValues;

		};
	}
}