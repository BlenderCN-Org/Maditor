#include "maditormodellib.h"

#include "Module.h"
#include "Project.h"
#include "Generators\HeaderGuardGenerator.h"
#include "ModuleList.h"
#include "DialogManager.h"



namespace Maditor {
	namespace Model {
		Module::Module(ModuleList *parent, const QString & name) :
			ProjectElement(name, "Module", parent),
			mParent(parent),
			mCmake(parent->cmake(), name)
		{
			
			init();

			QDir().mkdir(path());

			Generators::HeaderGuardGenerator header(this, name);
			header.generate();

		}

		Module::Module(QDomElement data, ModuleList *parent) :
			ProjectElement(data, parent),
			mParent(parent),
			mCmake(parent->cmake(), mName)
		{

			init();

			
			for (QDomElement generator = element().firstChildElement("Class"); !generator.isNull(); generator = generator.nextSiblingElement("Class")) {
				addClassImpl(Generators::ClassGeneratorFactory::load(this, generator), false);				
			}

			for (QDomElement dependency = element().firstChildElement("Dependency"); !dependency.isNull(); dependency = dependency.nextSiblingElement("Dependency")) {
				mCmake.addDependency(dependency.attribute("name"));
			}

			for (QDomElement libraryDependency = element().firstChildElement("LibraryDependency"); !libraryDependency.isNull(); libraryDependency = libraryDependency.nextSiblingElement("LibraryDependency")) {
				mCmake.addLibraryDependency(libraryDependency.attribute("name"));
			}

		}

		void Module::init()
		{
			setContextMenuItems({ 
				{ "New Class", [this]() {newClass(); } }
			});

			mCmake.addFile(Generators::HeaderGuardGenerator::fileName(mName));
		}

		QString Module::path() const
		{
			return mParent->path() + mName + "/";
		}
		void Module::newClass()
		{
			Generators::ClassGenerator *gen = Generators::ClassGeneratorFactory::createClass(this);
			if (gen) {
				addClass(gen);
			}
		}
		void Module::addClass(Generators::ClassGenerator * generator)
		{
			addClassImpl(generator);

			mParent->generate();

			project()->writeToDisk();
		}

		void Module::addClassImpl(Generators::ClassGenerator * generator, bool callInsert)
		{
			if (callInsert)
				project()->model()->beginInsertRows(ownIndex(), mClasses.size(), mClasses.size());
			mClasses.emplace_back(generator);

			mCmake.addFiles(generator->fileNames());

			if (callInsert)
				project()->model()->endInsertRows();

			emit classAdded(generator);
		}

		QString Module::moduleInclude()
		{
			return Generators::HeaderGuardGenerator::fileName(mName);
		}

		bool Module::hasClass(const QString & name)
		{
			return std::find_if(mClasses.begin(), mClasses.end(), [&](const std::unique_ptr<Generators::ClassGenerator> &ptr) {return ptr->name() == name; }) != mClasses.end();
		}

		QVariant Module::icon() const
		{
			QIcon icon;
			icon.addPixmap(QApplication::style()->standardPixmap(QStyle::SP_DirClosedIcon),
				QIcon::Normal, QIcon::Off);
			icon.addPixmap(QApplication::style()->standardPixmap(QStyle::SP_DirOpenIcon),
				QIcon::Normal, QIcon::On);
			return icon;
		}

		ModuleList * Module::parent()
		{
			return mParent;
		}

		bool Module::addDependency(const QString & dep)
		{
			Module *other = mParent->getModule(dep);

			std::set<const Module*> modules;
			fillReloadOrder(modules);
			if (std::find(modules.begin(), modules.end(), other) != modules.end()) {
				return false;
			}

			mCmake.addDependency(dep);
			
			mDependencies.insert(other);
			other->mDependedBy.insert(this);	

			dirty();

			return true;
		}

		void Module::removeDependency(const QString & dep)
		{

			mCmake.removeDependency(dep);

			Module *other = mParent->getModule(dep);
			mDependencies.erase(other);
			other->mDependedBy.erase(this);
			
			dirty();
		}

		const QStringList & Module::dependencies() const
		{
			return mCmake.dependencies();
		}

		const std::set<Module*> &Module::dependencyModules()
		{
			return mDependencies;
		}

		void Module::initDependencies()
		{
			for (const QString &dep : mCmake.dependencies()) {
				Module *other = mParent->getModule(dep);
				mDependencies.insert(other);
				other->mDependedBy.insert(this);
			}
		}

		void Module::fillReloadOrder(std::set<const Module*>& reloadOrder) const
		{
			if (reloadOrder.find(this) == reloadOrder.end()) {
				reloadOrder.insert(this);
				for (Module *m : mDependedBy) {
					m->fillReloadOrder(reloadOrder);
				}				
			}
		}

		int Module::childCount() const {
			return mClasses.size();
		}

		Generators::ClassGenerator *Module::child(int i) {
			auto it = mClasses.begin();
			std::advance(it, i);
			return it->get();
		}

		Generators::ClassGenerator * Module::getClass(const QString & name)
		{
			auto it = std::find_if(mClasses.begin(), mClasses.end(), [&](const std::unique_ptr<Generators::ClassGenerator> &c) {return c->name() == name; });
			if (it == mClasses.end())
				return nullptr;
			return it->get();
		}

		Project * Module::project()
		{
			return mParent->project();
		}

		const std::list<std::unique_ptr<Generators::ClassGenerator>> &Module::getClasses()
		{
			return mClasses;
		}

		void Module::deleteClass(Generators::ClassGenerator * generator)
		{
			bool deleteFiles;
			if (DialogManager::showDeleteClassDialogStatic(generator, deleteFiles)) {
				if (deleteFiles) {
					for (const QString &file : generator->filePaths()) {
						QFile f(file);
						f.remove();
					}
				}
				removeClass(generator);
			}
		}

		void Module::removeClass(Generators::ClassGenerator * generator)
		{
			auto it = std::find_if(mClasses.begin(), mClasses.end(), [=](const std::unique_ptr<Generators::ClassGenerator> &p) {return p.get() == generator; });
			if (it == mClasses.end())
				throw 0;
			size_t i = std::distance(mClasses.begin(), it);

			project()->model()->beginRemoveRows(ownIndex(), i, i);
			mCmake.removeFiles(generator->fileNames());
			
			mClasses.erase(it);			

			project()->model()->endRemoveRows();

			mParent->generate();

			project()->writeToDisk();
			
		}
		
	}
}