#pragma once


#include "ProjectElement.h"

#include "Generators\CmakeProject.h"

#include "Module.h"

namespace Maditor {
	namespace Model {
		class MADITOR_MODEL_EXPORT ModuleList : public QObject, public ProjectElement{
			Q_OBJECT

		public:

			ModuleList(Project *parent);
			ModuleList(QDomElement element, Project *parent);
			~ModuleList();

			virtual QString path() const override;

			void generate();
			void release();

			Generators::CmakeProject *cmake();

			Generators::ClassGenerator *getClass(const QString &fullName);

			bool hasModule(const QString &name) const;

			Module *getModule(const QString &name);
			const Module *getModule(const QString &name) const;

			std::list<Module *> initDependencies();

			std::list<std::unique_ptr<Module>>::const_iterator begin() const;
			std::list<std::unique_ptr<Module>>::const_iterator end() const;

			virtual QVariant icon() const override;

			virtual Project *project() override;

			// Inherited via ProjectElement
			virtual int childCount() const override;

			virtual Module * child(int i) override;

			void showPropertiesDialog();

		public slots:
			void createModule(const QString &name);
			void drawDependenciesGraph();

		private:
			void init();

			void addModule(Module *module);
			void newModule();

		signals:
			void moduleAdded(Module *);
			void classAdded(Generators::ClassGenerator*);


		private:
			Project *mParent;

			QString mPath;

			Generators::CmakeProject mCmake;

			std::list<std::unique_ptr<Module>> mModules;



		};
	}
}