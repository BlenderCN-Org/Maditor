#include "maditormodellib.h"

#include "ModuleLoader.h"

#include "Model\Project\Project.h"

#include "Model\Project\Module.h"

#include "Model\Project\ModuleList.h"

#include "ApplicationLauncher.h"

namespace Maditor {
	namespace Model {


		ModuleLoader::ModuleLoader(const QString & binaryDir, const ModuleList &moduleList) :			
			mBinaryDir(binaryDir),
			mModules(moduleList),
			mInstances(this, &ModuleLoader::createModule),
			setupDone(this, &ModuleLoader::setupDoneImpl),
			TableUnit(2)
		{
			setContainer(mInstances);
			/*const Shared::ModuleInstance *ptr = &*it;
			it2->setCallback([&, ptr](int column) {
				auto it = std::find_if(mInstances.begin(), mInstances.end(), [=](const Shared::ModuleInstance &module) {return &module == ptr; });
				dataChanged(std::distance(mInstances.begin(), it), column);
			})*/
						
			QDir dir(binaryDir);
			mFiles = QSet<QString>::fromList(dir.entryList({ "*.dll" }, QDir::NoDotAndDotDot | QDir::Files));


			connect(&mWatcher, &QFileSystemWatcher::fileChanged, this, &ModuleLoader::onFileChanged);
			connect(&mWatcher, &QFileSystemWatcher::directoryChanged, this, &ModuleLoader::onFolderChanged);

			mWatcher.addPath(binaryDir);
		}


		ModuleLoader::~ModuleLoader()
		{
			if (!mBinaryDir.isEmpty())
				mWatcher.removePath(mBinaryDir);

			std::list<const Module*> reloadOrder;

			for (const Shared::ModuleInstance &mod : mInstances) {
				mWatcher.removePath(mBinaryDir + QString::fromStdString(mod.name()) + ".dll");
			}

			mInstances.clear();

		}

		void ModuleLoader::reset()
		{
			mInstances.clear();
		}

		void ModuleLoader::onFileChanged(const QString &path) {
			return;

			QFileInfo f(path);
			const Module *m = mModules.getModule(f.baseName());
			auto it = mMap.find(m);
			//if (it == mInstances.end())
			//	return;
			Shared::ModuleInstance &module = *it->second;
			module.setExists(f.exists());
			if (module.exists()) {
				qDebug() << path << "Changed!";
				reload(m);
			}
			else {
				qDebug() << path << "was Deleted!";
			}
		}


		void ModuleLoader::onFolderChanged(const QString & path)
		{
			return;
			
			const QDir dir(path);

			QSet<QString> newEntryList = QSet<QString>::fromList(dir.entryList({ "*.dll" }, QDir::NoDotAndDotDot | QDir::Files));

			// Files that have been added
			QSet<QString> newFiles = newEntryList - mFiles;
			QStringList newFileList = newFiles.toList();

			//qDebug() << newEntryList << mFiles;

			// Update the current set
			mFiles = newEntryList;

			for (const QString &file : newFileList) {
				QFileInfo f(file);
				if (mModules.hasModule(f.baseName())) {
					reload(mModules.getModule(f.baseName()));
				}
			}

		}


		

		void ModuleLoader::addModule(Module *module)
		{
			
			mInstances.emplace_back_safe([=](const decltype(mInstances)::iterator &it) {
				QString path = mBinaryDir + module->name() + ".dll";
				QFile file(path);
				
				mMap[module] = &*it;
				it->setExists(file.exists());

				if (file.exists())
					mWatcher.addPath(path);

			}, this, module->name().toStdString());
		}


		void ModuleLoader::setup()
		{
			for (const std::unique_ptr<Module> &module : mModules) {
				addModule(module.get());
			}

		}

		void ModuleLoader::setup2()
		{
			for (const std::pair<const Model::Module *, Shared::ModuleInstance *> &module : mMap) {
				for (const QString &dep : module.first->dependencies()) {
					module.second->addDependency(mMap.at(mModules.getModule(dep)));
				}
			}
			setupDone();
		}

		bool ModuleLoader::done()
		{
			return mInstances.size() == mModules.childCount();
		}

		void ModuleLoader::notifyDataChanged(ModuleImpl * module, int column)
		{
			int row = std::distance(mInstances.begin(), std::find_if(mInstances.begin(), mInstances.end(), [=](const ModuleImpl &mod) { return &mod == module; }));
			QModelIndex i = index(row, column);
			emit dataChanged(i, i);
		}

		void ModuleLoader::reload(const Module *module) {

			mMap.at(module)->reload();
		}

		void ModuleLoader::setupDoneImpl()
		{
		}

		Q_INVOKABLE int ModuleLoader::rowCount(const QModelIndex & parent) const
		{
			return Q_INVOKABLE mInstances.size();
		}

		QVariant ModuleLoader::data(int row, int column, int role) const
		{
			auto it = mInstances.begin();
			std::advance(it, row);
			switch (role) {
			case Qt::DisplayRole:
				switch (column) {
				case 0:
					return QString::fromStdString(it->name());
				case 1:
					return it->isLoaded();
				}
				break;
			}
			return Q_INVOKABLE QVariant();
		}

		std::tuple<ModuleLoader*, std::string> ModuleLoader::createModule(const std::string & name)
		{
			return{ this, name };
		}

		ModuleImpl::ModuleImpl(ModuleLoader * loader, const std::string & name) :
			ModuleInstance(name)
		{
			mLoaded.setCallback([=](const bool &) { loader->notifyDataChanged(this, 1); });
		}

}
}