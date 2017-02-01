#pragma once

#include "OgreLog.h"


namespace Maditor {
	namespace Model {

		enum MessageType {
			ERROR_TYPE,
			WARNING_TYPE,
			LOG_TYPE
		};

			class MADITOR_MODEL_EXPORT LogTableModel : public QAbstractTableModel {
				Q_OBJECT

			public:
				LogTableModel();

			public slots:
				void addMessage(const QString &msg, MessageType level, const QString &traceback, const std::string &fileName, int lineNr);
				void doubleClicked(const QModelIndex &index);
				void clear();

			public:

				// Inherited via QAbstractTableModel
				virtual Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const override;

				virtual Q_INVOKABLE int columnCount(const QModelIndex & parent = QModelIndex()) const override;

				virtual Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;


			private:
				std::list<std::tuple<MessageType, QString, QString, std::string, int>> mItems;
				
			};

	}
}