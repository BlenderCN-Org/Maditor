#pragma once

#include "ProjectElement.h"

namespace Maditor {
	namespace Model {
		class MADITOR_MODEL_EXPORT ModuleSelection : public ProjectElement, public QAbstractListModel {
		public:
			ModuleSelection(ApplicationConfig *parent);
			ModuleSelection(QDomElement element, ApplicationConfig *parent);

			// Geerbt �ber QAbstractListModel
			virtual Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const override;

			virtual Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

			bool storeData() override;
			void restoreData() override;

		public slots:
			void itemClicked(const QModelIndex &index);

		private:
			ModuleList *mModules;

			ApplicationConfig *mParent;
			
			std::list<QString> mExcludedModules;

		};
	}
}