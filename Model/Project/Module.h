
#pragma once

#include "Generators\CmakeSubProject.h"
#include "Generators\ClassGenerator.h"
#include "ProjectElement.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT Module : public QObject, public ProjectElement {
			Q_OBJECT
		public:
			Module(ModuleList *parent, const QString &name);
			Module(QDomElement data, ModuleList *parent);

			virtual QString path() const override;

			
			
			QString moduleInclude();

			bool hasClass(const QString &name);

			virtual QVariant icon() const override;
			

			bool addDependency(const QString &dep);
			void removeDependency(const QString &dep);
			const QStringList &dependencies() const;
			const std::set<Module *> &dependencyModules();
			void initDependencies();

			void fillReloadOrder(std::list<const Module*> &reloadOrder) const;

			ModuleList *parent();

			// Inherited via ProjectElement
			virtual int childCount() const override;

			virtual Generators::ClassGenerator * child(int i) override;

			virtual Project *project() override;

			const std::list<std::unique_ptr<Generators::ClassGenerator>> &getClasses();

			void newClass();
			void addClass(Generators::ClassGenerator *generator);
			void deleteClass(Generators::ClassGenerator *generator);
			void removeClass(Generators::ClassGenerator *generator);

		private:
			void addClassImpl(Generators::ClassGenerator *generator, bool callInsert = true);

			void init();

			void showPropertiesDialog();

		signals:

			void classAdded(Generators::ClassGenerator *generator);

		private:
			ModuleList *mParent;

			Generators::CmakeSubProject mCmake;

			std::list<std::unique_ptr<Generators::ClassGenerator>> mClasses;
			
			std::set<Module *> mDependencies, mDependedBy;



		};

	}
}