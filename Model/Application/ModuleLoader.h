#pragma once

#include "Serialize\Container\list.h"

#include "Shared\moduleinstance.h"

#include "Serialize\Container\action.h"

#include "TableUnit.h"

namespace Maditor {
	namespace Model {

		class ModuleImpl : public Shared::ModuleInstance {
		public:
			ModuleImpl(ModuleLoader *loader, const std::string &name);
		};

		class ModuleLoader : public TableUnit {
			Q_OBJECT

		public:
			ModuleLoader(const QString &binaryDir, const ModuleList &moduleList);
			~ModuleLoader();

			void reset();

			void setup();
			void setup2();

			bool done();

			void notifyDataChanged(ModuleImpl *module, int column);

			// Geerbt �ber TableUnit
			virtual Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const override;

			virtual QVariant data(int row, int column, int role = Qt::DisplayRole) const override;


		private slots:
			void onFileChanged(const QString &path);
			void onFolderChanged(const QString &path);

		private:
			void addModule(Module *module);

			void reload(const Module *module);

			void setupDoneImpl();

			std::tuple<ModuleLoader *, std::string> createModule(const std::string &name);

		private:
			QFileSystemWatcher mWatcher;
			QString mBinaryDir;
			QSet<QString> mFiles;

			const ModuleList &mModules;

			std::map<const Module *, Shared::ModuleInstance*> mMap;
			Engine::Serialize::ObservableList<ModuleImpl, ModuleLoader*, std::string> mInstances;

			Engine::Serialize::Action<> setupDone;
			
			

		};
	}
}