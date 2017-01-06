#include "maditormodellib.h"

#include "TreeUnit.h"

#include "Serialize\Container\serializablecontainer.h"

namespace Maditor {
	namespace Model {

		TreeUnit::TreeUnit(int columnCount) :
			TreeModel(this, columnCount),
			TreeUnitItem(this)
		{

		}

		TreeUnitItem::TreeUnitItem(TreeUnit * tree) :
			mParent(0),
			mTree(tree)
		{
		}

		void TreeUnitItem::notifyDataChange(int column)
		{
			notifyDataChange(column, column);
		}

		void TreeUnitItem::notifyDataChange(int fromCol, int toCol)
		{
			mTree->onDataChanged(getIndex(), parentIndex(), fromCol, toCol);
		}

		TreeUnitItem::TreeUnitItem(TreeUnitItem * parent) :
			mParent(parent),
			mTree(parent->mTree)
		{
		}

		TreeUnitItem * TreeUnitItem::parentItem()
		{
			return mParent;
		}

		QModelIndex TreeUnitItem::getIndex()
		{
			return mParent ? mTree->index(parentIndex(), 0, mParent->getIndex()) : QModelIndex();
		}

		QVariant TreeUnit::data(int col) const
		{
			return QVariant();
		}

		void TreeUnit::handleOperation(const QModelIndex & parent, int row, int op)
		{
			using namespace Engine::Serialize;

			switch (op) {
			case BEFORE | CLEAR:
				beginResetModel();
				break;
			case AFTER | CLEAR:
				endResetModel();
				break;
			case INSERT_ITEM:
				beginInsertRows(parent, row, row);
				endInsertRows();
				break;
			default:
				throw 0;
			}
		}

		void TreeUnit::onDataChanged(const QModelIndex & parent, int row, int fromCol, int toCol)
		{
			emit dataChanged(index(row, fromCol, parent), index(row, toCol, parent));
		}

	}
}