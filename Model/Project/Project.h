#pragma once

#include "Generators\Generator.h"

#include "ProjectElement.h"

#include "Model\TreeModel.h"


namespace Maditor {
	namespace Model {
		class MADITOR_MODEL_EXPORT Project : public Document, public ProjectElement, public Generators::Generator {
			Q_OBJECT

		public:
			Project(LogsModel *logs, const QString &path, const QString &name, QDomDocument doc = QDomDocument());
			Project(LogsModel *logs, QDomDocument doc, const QString &path);
			~Project();

			static Project *load(LogsModel *logs, const QString &path);

			bool isValid();

			//virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
					
			bool writeToDisk();
			virtual void save() override;
			virtual void discardChanges() override;

			LogsModel *logs();
			ModuleList *moduleList();
			ConfigList *configList();
			QFileSystemModel *getMedia();
			TreeModel *model();

			virtual Project *project() override;
			virtual int childCount() const override;
			virtual ProjectElement* child(int i) override;
			virtual QString path() const override;
			virtual QStringList filePaths() override;
			virtual void write(QTextStream & stream, int index) override;		

			void release();

		signals:
			void showProperties();


		public slots:
			void deleteClass(Generators::ClassGenerator *generator, bool deleteFiles);

			void mediaDoubleClicked(const QModelIndex &index);

		private slots:
		    void onClassAdded(Generators::ClassGenerator *generator);

		private:
			void init();

			void copyTemplate(QMessageBox::StandardButton *answer);
		
		private:
			QDomDocument mDocument;
			
			QString mPath;

			bool mValid;

			static const QString sProjectFileName;

			std::unique_ptr<ModuleList> mModules;
			std::unique_ptr<ConfigList> mConfigs;

			QFileSystemModel mMediaFolder;

			LogsModel *mLogs;

			TreeModel mModel;

		};
	}
}