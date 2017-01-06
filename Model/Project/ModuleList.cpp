#include "maditormodellib.h"

#include "ModuleList.h"

#include "Project.h"

#include "Generators\DotGenerator.h"

#include "DialogManager.h"

namespace Maditor {
	namespace Model {

		
		ModuleList::ModuleList(Project *parent) :
			ProjectElement("C++", "Modules", parent),
			mPath(parent->path() + "src/"),
			mCmake(mPath, parent->path(), parent->name()),
			mParent(parent)
		{

		}

		ModuleList::ModuleList(QDomElement element, Project *parent) :
			ProjectElement(element, parent),
			mPath(parent->path() + "src/"),
			mCmake(mPath, parent->path(), parent->name()),
			mParent(parent)
		{
			init();

			for (QDomElement module = element.firstChildElement("Module"); !module.isNull(); module = module.nextSiblingElement("Module")) {
				addModule(new Module(module, this));
			}

			for (const std::unique_ptr<Module> &module : mModules)
				module->initDependencies();

			for (QDomElement library = element.firstChildElement("Library"); !library.isNull(); library = library.nextSiblingElement("Library")) {
				mCmake.addLibrary(library.attribute("name"));
			}
		}

		ModuleList::~ModuleList()
		{
		}

		void ModuleList::init()
		{
			setContextMenuItems({
				{ "New Module", [=]() {newModule(); } },
				{ "Rebuild Structure", [=]() {generate(); } },
				{ "Draw Dependencies", [=]() {drawDependenciesGraph(); } }
			});
		}

		void ModuleList::addModule(Module * module)
		{
			//beginInsertRows(index(0, 0), mModules.size(), mModules.size());
			mModules.emplace_back(module);
			//endInsertRows();

			emit moduleAdded(module);
		}

		void ModuleList::newModule()
		{
			QString name;
			if (DialogManager::showNewModuleDialogStatic(this, name)) {
				createModule(name);
			}
		}

		void ModuleList::drawDependenciesGraph()
		{
			Generators::DotGenerator dot(mParent->path() + "tmp/", "Dependencies");

			for (const std::unique_ptr<Module> &module : mModules) {
				dot.addNode(module->name());
				for (const QString &dependency : module->dependencies()) {
					dot.addEdge(module->name(), dependency);
				}
			}

			dot.generate();
		}

		QString ModuleList::path() const
		{
			return mPath;
		}

		void ModuleList::generate()
		{
			mCmake.generate();
			mCmake.build("debug");
		}

		void ModuleList::release()
		{
			mCmake.generate();
			mCmake.build("release");
		}

		void ModuleList::createModule(const QString & name)
		{
			addModule(new Module(this, name));

			generate();
			mParent->save();

		}


		Generators::CmakeProject * ModuleList::cmake()
		{
			return &mCmake;
		}

		bool ModuleList::hasModule(const QString & name) const
		{
			return std::find_if(mModules.begin(), mModules.end(), [&](const std::unique_ptr<Module> &ptr) {return ptr->name() == name; }) != mModules.end();
		}

		Module * ModuleList::getModule(const QString & name)
		{
			auto it = std::find_if(mModules.begin(), mModules.end(), [&](const std::unique_ptr<Module> &mod) {return mod->name() == name; });
			if (it == mModules.end())
				throw 0;
			return it->get();
		}

		const Module * ModuleList::getModule(const QString & name) const
		{
			auto it = std::find_if(mModules.begin(), mModules.end(), [&](const std::unique_ptr<Module> &mod) {return mod->name() == name; });
			if (it == mModules.end())
				throw 0;
			return it->get();
		}

		std::list<Module*> ModuleList::initDependencies()
		{
			std::list<Module*> result;
			for (const QString &dependency : mCmake.dependencies())
				result.push_back(getModule(dependency));

			return result;
		}

		std::list<std::unique_ptr<Module>>::const_iterator ModuleList::begin() const
		{
			return mModules.begin();
		}

		std::list<std::unique_ptr<Module>>::const_iterator ModuleList::end() const
		{
			return mModules.end();
		}

		int ModuleList::childCount() const {
			return mModules.size();
		}

		Module * ModuleList::child(int i) {
			auto it = mModules.begin();
			std::advance(it, i);
			return it->get();
		}

		QVariant ModuleList::icon() const
		{
			QIcon icon;
			icon.addPixmap(QApplication::style()->standardPixmap(QStyle::SP_DirHomeIcon),
				QIcon::Normal);
			return icon;
		}

		Project * ModuleList::project()
		{
			return mParent;
		}

	}
}