#pragma once

#include "Serialize\Container\list.h"

#include "Shared\moduleinstance.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Model {


		class ModuleLoader : public QObject, public Engine::Serialize::SerializableUnit {
			Q_OBJECT

		public:
			ModuleLoader(ApplicationLauncher *launcher, const QString &binaryDir, const ModuleList &moduleList);
			~ModuleLoader();


			void setup();


		private slots:
			void onFileChanged(const QString &path);
			void onFolderChanged(const QString &path);

		private:
			void addModule(Module *module);
			
			void loadModule(Shared::ModuleInstance &module, bool callInit);
			void unloadModule(Shared::ModuleInstance &module);
			void reload(const Module *module);

			void setupDoneImpl();

		private:
			QFileSystemWatcher mWatcher;
			QString mBinaryDir;
			QSet<QString> mFiles;

			const ModuleList &mModules;

			std::map<const Module *, Shared::ModuleInstance*> mMap;
			Engine::Serialize::ObservableList<Shared::ModuleInstance, std::string> mInstances;

			Engine::Serialize::Action<> setupDone;
			
			
		};
	}
}