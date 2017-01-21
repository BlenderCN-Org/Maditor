#include "maditormodellib.h"

#include "Module.h"
#include "Project.h"
#include "Generators\HeaderGuardGenerator.h"
#include "ModuleList.h"
#include "DialogManager.h"

#include "Generators\GuiHandlerGenerator.h"
#include "Generators\GameHandlerGenerator.h"
#include "Generators\GlobalAPIGenerator.h"
#include "Generators\SceneComponentGenerator.h"
#include "Generators\OtherClassGenerator.h"
#include "Generators\EntityComponentGenerator.h"


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
				{ "New Class", [this]() {newClass(); } },
				{ "Properties", [this]() {showPropertiesDialog(); }}
			});

			mCmake.addFile(Generators::HeaderGuardGenerator::fileName(mName));
		}

		QString Module::path() const
		{
			return mParent->path() + mName + "/";
		}
		void Module::newClass()
		{
			Generators::ClassGeneratorFactory::ClassType type;
			QString name;
			if (DialogManager::showNewClassDialogStatic(this, name, type)) {
				switch (type) {
				case Generators::ClassGeneratorFactory::GUI_HANDLER:
				{
					QString window;
					int type;
					bool hasLayout;
					if (DialogManager::showNewGuiHandlerDialogStatic(this, name, window, type, hasLayout)) {
						addClass(new Generators::GuiHandlerGenerator(this, name, window, type, hasLayout));
					}
				}
					break;
				case Generators::ClassGeneratorFactory::GAME_HANDLER:
					if (DialogManager::showNewGameHandlerDialogStatic(this, name)) {
						addClass(new Generators::GameHandlerGenerator(this, name));
					}
					break;
				case Generators::ClassGeneratorFactory::ENTITY_COMPONENT:
				{
					QString componentName;
					if (DialogManager::showNewEntityComponentDialogStatic(this, name, componentName)) {
						addClass(new Generators::EntityComponentGenerator(this, name, componentName));
					}
				}
					break;
				case Generators::ClassGeneratorFactory::SCENE_COMPONENT:
					if (DialogManager::showNewSceneComponentDialogStatic(this, name)) {
						addClass(new Generators::SceneComponentGenerator(this, name));
					}
					break;
				case Generators::ClassGeneratorFactory::GLOBAL_API:
					if (DialogManager::showNewGlobalAPIDialogStatic(this, name)) {
						addClass(new Generators::GlobalAPIGenerator(this, name));
					}
					break;
				case Generators::ClassGeneratorFactory::OTHER_CLASS:
					if (DialogManager::showNewOtherClassDialogStatic(this, name)) {
						addClass(new Generators::OtherClassGenerator(this, name));
					}
					break;
				default:
					throw 0;
				}
			}
		}
		void Module::addClass(Generators::ClassGenerator * generator)
		{
			addClassImpl(generator);

			mParent->generate();

			project()->save();
		}
		void Module::addClassImpl(Generators::ClassGenerator * generator, bool callInsert)
		{
			if (callInsert)
				project()->beginInsertRows(ownIndex(), mClasses.size(), mClasses.size());
			mClasses.emplace_back(generator);

			mCmake.addFiles(generator->fileNames());

			if (callInsert)
				project()->endInsertRows();

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

		void Module::showPropertiesDialog()
		{
			DialogManager::showModulePropertiesDialogStatic(this);
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
			std::list<const Module*> modules;
			fillReloadOrder(modules);
			if (std::find(modules.begin(), modules.end(), other) != modules.end()) {
				return false;
			}

			QDomElement el = document().createElement("Dependency");
			el.setAttribute("name", dep);
			element().appendChild(el);

			mCmake.addDependency(dep);

			
			mDependencies.insert(other);
			other->mDependedBy.insert(this);	

			return true;
		}

		void Module::removeDependency(const QString & dep)
		{
			bool found = false;
			for (QDomElement el = element().firstChildElement("Dependency"); !el.isNull() && !found; el = el.nextSiblingElement("Dependency")) {
				if (el.attribute("name") == dep) {
					element().removeChild(el);
					found = true;
				}
			}

			if (!found)
				throw 0;			

			mCmake.removeDependency(dep);

			Module *other = mParent->getModule(dep);
			mDependencies.erase(other);
			other->mDependedBy.erase(this);

			
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

		void Module::fillReloadOrder(std::list<const Module*>& reloadOrder) const
		{
			if (std::find(reloadOrder.begin(), reloadOrder.end(), this) == reloadOrder.end()) {
				for (Module *m : mDependedBy) {
					m->fillReloadOrder(reloadOrder);
				}
				reloadOrder.push_back(this);
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

			project()->beginRemoveRows(ownIndex(), i, i);
			mCmake.removeFiles(generator->fileNames());
			
			mClasses.erase(it);			

			project()->endRemoveRows();

			mParent->generate();

			project()->save();
			
		}
		
	}
}