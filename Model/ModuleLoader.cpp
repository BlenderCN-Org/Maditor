#include "maditormodellib.h"

#include "ModuleLoader.h"

#include "Model\Project\Project.h"

#include "Model\Project\Module.h"

#include "Model\Project\ModuleList.h"


namespace Maditor {
	namespace Model {


		ModuleLoader::ModuleLoader(const QString & binaryDir, const ModuleList &moduleList) :
			mBinaryDir(binaryDir),
			mModules(moduleList)
		{
						

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

			for (const std::pair<const Module* const, ModuleInstance> &p : mInstances) {
				mWatcher.removePath(mBinaryDir + p.first->name() + ".dll");
			}

			mInstances.clear();

		}

		void ModuleLoader::onFileChanged(const QString &path) {
			QFileInfo f(path);
			const Module *m = mModules.getModule(f.baseName());
			auto it = mInstances.find(m);
			//if (it == mInstances.end())
			//	return;
			ModuleInstance &module = it->second;
			module.mExists = f.exists();
			if (module.mExists) {
				qDebug() << path << "Changed!";
				reload(m);
			}
			else {
				qDebug() << path << "was Deleted!";
			}
		}


		void ModuleLoader::onFolderChanged(const QString & path)
		{
			
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
			QString path = mBinaryDir + module->name() + ".dll";
			QFile file(path);

			ModuleInstance &mod = mInstances.emplace(module, module->name()).first->second;
			mod.mExists = file.exists();

			if (mod.mExists)
				mWatcher.addPath(path);			
		}

		void ModuleLoader::sendAll()
		{
			std::list<const Module*> reloadOrder;
			for (const std::unique_ptr<Module> &module : mModules) {
				addModule(module.get());
				module->fillReloadOrder(reloadOrder);
			}

			reloadOrder.reverse();

			for (const Module *m : reloadOrder) {
				ModuleInstance &instance = mInstances.at(m);

				//loadModule(instance, false);
			}

			/*ModuleLoaderMsg msg;
			msg.mCmd = Init;

			sendMsg(msg, "Loader");*/
		}

		void ModuleLoader::reload(const Module *module) {
			std::list<const Module*> reloadOrder;
			module->fillReloadOrder(reloadOrder);

			for (const Module *m : reloadOrder) {
				ModuleInstance &instance = mInstances.at(m);

				//unloadModule(instance);
			}
			
			reloadOrder.reverse();

			for (const Module *m : reloadOrder) {
				ModuleInstance &instance = mInstances.at(m);

				//loadModule(instance, true);
			}
		}


		

	}
}