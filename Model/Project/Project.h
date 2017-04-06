#pragma once

#include "Generators\Generator.h"

#include "ProjectElement.h"

#include "Model\TreeModel.h"


namespace Maditor {
	namespace Model {
		class MADITOR_MODEL_EXPORT Project : public TreeModel, public ProjectElement, public Generators::Generator {
			Q_OBJECT

		public:
			Project(LogsModel *logs, const QString &path, const QString &name, QDomDocument doc = QDomDocument());
			Project(LogsModel *logs, QDomDocument doc, const QString &path);
			~Project();

			static Project *load(LogsModel *logs, const QString &path);

			bool isValid();

			virtual QString path() const override;			

			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
					
			void save();

			LogsModel *logs();
			ModuleList *moduleList();
			ConfigList *configList();

			virtual Project *project() override;

			// Inherited via ProjectElement
			virtual int childCount() const override;

			virtual ProjectElement* child(int i) override;

			QFileSystemModel *getMedia();

			void release();

			// Geerbt über Generator
			virtual QStringList filePaths() override;

			virtual void write(QTextStream & stream, int index) override;


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

		};
	}
}