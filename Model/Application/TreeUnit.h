#pragma once

#include "Model/TreeModel.h"
#include "Model/TreeItem.h"

#include "Serialize\serializableunit.h"

namespace Maditor {
	namespace Model {

		class TreeUnitItem : public TreeItem, public Engine::Serialize::SerializableUnit {
		public:
			TreeUnitItem(TreeUnitItem *parent);

			virtual TreeUnitItem *parentItem() override;

			QModelIndex getIndex();

		protected:
			TreeUnitItem(TreeUnit *tree);

			void notifyDataChange(int column);
			void notifyDataChange(int fromCol, int toCol);

			template <class C>
			void setContainer(C &container) {
				container.setCallback([&container = (const C&)container, this](const typename C::const_iterator &it, int op) {
					int row = std::distance(container.begin(), it);
					mTree->handleOperation(getIndex(), row, op);
				});
			}

		private:
			TreeUnitItem *mParent;
			TreeUnit *mTree;
		};

		class TreeUnit : public TreeModel, public TreeUnitItem{
			Q_OBJECT

		public:
			
			TreeUnit(int columnCount);

			virtual QVariant data(int column) const override;

			void handleOperation(const QModelIndex &parent, int row, int op);
			void onDataChanged(const QModelIndex &parent, int row, int fromCol, int toCol);

		};
	}
}