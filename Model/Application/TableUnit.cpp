#include "maditormodellib.h"

#include "TableUnit.h"

#include "Serialize\Container\serializablecontainer.h"

namespace Maditor {
	namespace Model {

		TableUnitBase::TableUnitBase(int columnCount) :
			TableModel(columnCount)
		{

		}

		void TableUnitBase::handleOperation(int row, int op)
		{
			using namespace Engine::Serialize;

			switch (op) {
			case BEFORE | RESET:
				beginResetModel();
				break;
			case AFTER | RESET:
				endResetModel();
				break;
			case INSERT_ITEM:
				beginInsertRows(QModelIndex(), row, row);
				endInsertRows();
				break;
			default:
				throw 0;
			}
		}

	}
}