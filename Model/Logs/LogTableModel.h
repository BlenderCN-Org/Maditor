#pragma once

#include "Util\UtilMethods.h"


namespace Maditor {
	namespace Model {

		

			class MADITOR_MODEL_EXPORT LogTableModel : public QAbstractTableModel {
				Q_OBJECT

			public:
				LogTableModel();

			public slots:
				void addMessage(const QString &msg, Util::MessageType level, const QString &traceback, const std::string &fileName, int lineNr);
				void doubleClicked(const QModelIndex &index);
				void clear();

			public:

				// Inherited via QAbstractTableModel
				virtual Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const override;

				virtual Q_INVOKABLE int columnCount(const QModelIndex & parent = QModelIndex()) const override;

				virtual Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;


			private:
				std::list<std::tuple<Util::MessageType, QString, QString, std::string, int>> mItems;
				
			};

	}
}