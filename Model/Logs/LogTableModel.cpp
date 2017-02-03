#include "maditormodellib.h"

#include "LogTableModel.h"

//#include "Model\Editors\EditorManager.h"

namespace Maditor {
	namespace Model {

			LogTableModel::LogTableModel()
			{				
			}

			void LogTableModel::addMessage(const QString &msg, Engine::Util::MessageType level, const QString &traceback, const std::string &fileName, int lineNr) {
				beginInsertRows(QModelIndex(), 0, 0);
				mItems.emplace_front(level, msg, traceback, fileName, lineNr);
				endInsertRows();
			}

			void LogTableModel::doubleClicked(const QModelIndex & index)
			{
				auto it = mItems.begin();
				std::advance(it, index.row());

				const std::string &fileName = std::get<3>(*it);
			
				if (fileName == "<unknown>")
					return;

				//Editors::EditorManager::getSingleton().openByExtension(traceback.mFile, traceback.mLineNr);
			}

			void LogTableModel::clear()
			{
				beginResetModel();
				mItems.clear();
				endResetModel();
			}

			Q_INVOKABLE int LogTableModel::rowCount(const QModelIndex & parent) const
			{
				return mItems.size();
			}

			Q_INVOKABLE int LogTableModel::columnCount(const QModelIndex & parent) const
			{
				return 3;
			}

			Q_INVOKABLE QVariant LogTableModel::data(const QModelIndex & index, int role) const
			{
				if (!index.isValid())
					return QVariant();
				
				if (index.row() >= mItems.size())
					return QVariant();

				if (index.column() >= 3)
					return QVariant();



				auto it = mItems.begin();
				std::advance(it, index.row());

				if (index.column() == 0) {
					if (role != Qt::DecorationRole)
						return QVariant();
					QIcon icon;
					switch (std::get<0>(*it)) {
					case Engine::Util::LOG_TYPE:
						icon.addPixmap(QApplication::style()->standardPixmap(QStyle::SP_MessageBoxInformation));
						break;
					case Engine::Util::WARNING_TYPE:
						icon.addPixmap(QApplication::style()->standardPixmap(QStyle::SP_MessageBoxWarning));
						break;
					case Engine::Util::ERROR_TYPE:
						icon.addPixmap(QApplication::style()->standardPixmap(QStyle::SP_MessageBoxCritical));
						break;
					}
					return icon;
				}
				else if (index.column() == 2) {
					if (role != Qt::DisplayRole)
						return QVariant();
					return QVariant(std::get<1>(*it));
				}
				else if (index.column() == 1) {
					if (role != Qt::DisplayRole)
						return QVariant();
					return QVariant(std::get<2>(*it));
				}

				return QVariant();
			}


	}
}