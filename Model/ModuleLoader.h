#pragma once


namespace Maditor {
	namespace Model {


		class ModuleLoader : public QObject {
			Q_OBJECT

		public:
			ModuleLoader(const QString &binaryDir, const ModuleList &moduleList);
			~ModuleLoader();


		protected:
			void addModule(Module *module);		

			void sendAll();


		private slots:
			void onFileChanged(const QString &path);
			void onFolderChanged(const QString &path);

		private:
			QFileSystemWatcher mWatcher;
			QSet<QString> mFiles;

			struct ModuleInstance {
				ModuleInstance(const QString &name) :
					mExists(false),
					mName(name) {}

				bool mExists;
				QString mName;

			};

			void loadModule(ModuleInstance &module, bool callInit);
			void unloadModule(ModuleInstance &module);
			void reload(const Module *module);

			std::map<const Module *, ModuleInstance> mInstances;

			QString mBinaryDir;

			const ModuleList &mModules;

			
		};
	}
}