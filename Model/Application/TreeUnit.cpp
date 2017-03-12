#include "maditormodellib.h"

#include "TreeUnit.h"

#include "Serialize\Container\serializablecontainer.h"

namespace Maditor {
	namespace Model {

		TreeUnitBase::TreeUnitBase(int columnCount) :
			TreeModel(this, columnCount),
			TreeUnitItemBase(this)
		{

		}

		TreeUnitItemBase::TreeUnitItemBase(TreeUnitBase * tree) :
			mParent(0),
			mTree(tree)
		{
		}

		void TreeUnitItemBase::notifyDataChange(int column)
		{
			notifyDataChange(column, column);
		}

		void TreeUnitItemBase::notifyDataChange(int fromCol, int toCol)
		{
			mTree->onDataChanged(getIndex(), parentIndex(), fromCol, toCol);
		}

		TreeUnitItemBase::TreeUnitItemBase(TreeUnitItemBase * parent) :
			mParent(parent),
			mTree(parent->mTree)
		{
		}

		TreeUnitItemBase * TreeUnitItemBase::parentItem()
		{
			return mParent;
		}

		QModelIndex TreeUnitItemBase::getIndex()
		{
			return mParent ? mTree->index(parentIndex(), 0, mParent->getIndex()) : QModelIndex();
		}

		QVariant TreeUnitBase::data(int col) const
		{
			return QVariant();
		}

		void TreeUnitBase::handleOperation(const QModelIndex & parent, int row, int op)
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
			case BEFORE | SET_FULL_STATE:
				beginResetModel();
				break;
			case AFTER | SET_FULL_STATE:
				endResetModel();
				break;
			default:
				throw 0;
			}
		}

		void TreeUnitBase::onDataChanged(const QModelIndex & parent, int row, int fromCol, int toCol)
		{
			emit dataChanged(index(row, fromCol, parent), index(row, toCol, parent));
		}

	}
}