
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

			void fillReloadOrder(std::set<const Module*> &reloadOrder) const;

			ModuleList *parent();

			// Inherited via ProjectElement
			virtual int childCount() const override;

			virtual Generators::ClassGenerator * child(int i) override;
			Generators::ClassGenerator *getClass(const QString &name);

			virtual Project *project() override;

			template <class T>
			T *getClass(const QString &name) {
				auto it = std::find_if(mClasses.begin(), mClasses.end(), [&](const std::unique_ptr<Generators::ClassGenerator> &c) {return c->name() == name; });
				if (it == mClasses.end())
					return nullptr;
				return dynamic_cast<T*>(it->get());
			}
			const std::list<std::unique_ptr<Generators::ClassGenerator>> &getClasses();
			template <class T>
			std::list<T*> getClasses() {
				std::list<T*> result;
				for (const std::unique_ptr<Generators::ClassGenerator> &classGen : mClasses) {
					if (T *t = dynamic_cast<T*>(classGen.get()))
						result.push_back(t);
				}
				return result;
			}

			void newClass();
			void addClass(Generators::ClassGenerator *generator);
			void deleteClass(Generators::ClassGenerator *generator);
			void removeClass(Generators::ClassGenerator *generator);

		private:
			void addClassImpl(Generators::ClassGenerator *generator, bool callInsert = true);

			void init();

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