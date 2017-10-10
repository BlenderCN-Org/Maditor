#pragma once

#include "Model/TreeModel.h"
#include "Model/TreeItem.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {

		class TreeUnitItemBase : public TreeItem {
		public:
			TreeUnitItemBase(TreeUnitItemBase *parent);

			virtual TreeUnitItemBase *parentItem() const override;

			QModelIndex getIndex();

		protected:
			TreeUnitItemBase(TreeUnitBase *tree);

			void notifyDataChange(int column);
			void notifyDataChange(int fromCol, int toCol);

			template <class C>
			void setContainer(C &container) {
				container.connectCallback([&container = (const C&)container, this](const typename C::const_iterator &it, int op) {
					int row = std::distance(container.begin(), it);
					handleOperation(row, op);
				});
			}

			void handleOperation(int row, int op);

		private:
			TreeUnitItemBase *mParent;
			TreeUnitBase *mTree;
		};



		class TreeUnitBase : public TreeModel, public TreeUnitItemBase {
			Q_OBJECT

		public:
			
			TreeUnitBase(int columnCount);

			virtual QVariant cellData(int column) const override;

			void handleOperation(const QModelIndex &parent, int row, int op);
			void onDataChanged(const QModelIndex &parent, int row, int fromCol, int toCol);

		};

		template <class T>
		class TreeUnit : public TreeUnitBase, public Engine::Serialize::SerializableUnit<T> {
		public:
			TreeUnit(Engine::Serialize::SerializableUnitBase *parent, int columnCount) :
				TreeUnitBase(columnCount),
				SerializableUnit(parent) {}
		};

		template <class T>
		class TreeUnitItem : public Engine::Serialize::SerializableUnit<T>, public TreeUnitItemBase {
		public:
			template <class U>
			TreeUnitItem(TreeUnitItem<U> *parent) :
				SerializableUnit(parent),
				TreeUnitItemBase(parent) {}

		protected:
			template <class U>
			TreeUnitItem(TreeUnit<U> *tree) :
				SerializableUnit(tree),
				TreeUnitItemBase(static_cast<TreeUnitItemBase*>(tree)) {}
		};

	}
}