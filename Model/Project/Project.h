#pragma once

#include "Generators\Generator.h"

#include "ProjectElement.h"

#include "Model\TreeModel.h"


namespace Maditor {
	namespace Model {
		class MADITOR_MODEL_EXPORT Project : public TreeModel, public ProjectElement, public Generators::Generator {
			Q_OBJECT

		public:
			Project(const QString &path, const QString &name, QDomDocument doc = QDomDocument());
			Project(QDomDocument doc, const QString &path);
			~Project();

			static Project *load(const QString &path);

			bool isValid();

			virtual QString path() const override;			

			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
					
			void save();

			ModuleList *moduleList();

			virtual Project *project() override;

			// Inherited via ProjectElement
			virtual int childCount() const override;

			virtual ProjectElement* child(int i) override;

			QFileSystemModel *getMedia();

			void release();

			// Geerbt �ber Generator
			virtual QStringList filePaths() override;

			virtual void write(QTextStream & stream, int index) override;

			ApplicationLauncher *application();

		public slots:
			void deleteClass(Generators::ClassGenerator *generator, bool deleteFiles);

			void mediaDoubleClicked(const QModelIndex &index);

		private:
			void init();

			void copyTemplate(QMessageBox::StandardButton *answer);

			

		
		private:
			QDomDocument mDocument;
			
			QString mPath;

			bool mValid;

			static const QString sProjectFileName;

			ModuleList *mModules;

			QFileSystemModel mMediaFolder;

			ApplicationLauncher *mApplication;


		};
	}
}