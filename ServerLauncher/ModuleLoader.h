#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Launcher {

class ModuleLoader : public Engine::Serialize::SerializableUnit{

public:
	ModuleLoader();
	~ModuleLoader();

	void setup(const std::string &binaryDir, const std::string &runtimeDir);
	void cleanup();
			
	bool receiving();

	const std::string &binaryDir();
	const std::string &runtimeDir();

	Engine::Server::BaseServer *createServer(const std::string &fullName);

private:

	class ModuleLauncherInstance : public Shared::ModuleInstance {
	public:
		ModuleLauncherInstance(ModuleLoader *parent, const std::string &name);

		~ModuleLauncherInstance();

		void createDependencies();
		bool load(bool callInit);
		bool unload();

		virtual void reloadImpl() override;

		void *getSymbol(const std::string symbolName);

	private:
		HINSTANCE mHandle;

		std::list<ModuleLauncherInstance *> mDependedBy;

		std::list<std::string> mEntityComponentNames;
		std::list<void*> mSceneComponentHashes;
		std::list<void*> mGlobalAPIComponentHashes;
		std::map<std::string, std::list<Engine::Scene::Entity::Entity*>> mStoredComponentEntities;

		ModuleLoader *mParent;
	};

	void setupDoneImpl();
	
private:
	std::tuple<ModuleLoader *, std::string> createModule(const std::string &name);

private:
	std::string mBinaryDir, mRuntimeDir;

	bool mInit;

	bool mReceivingModules;

	Engine::Serialize::ObservableList<ModuleLauncherInstance, Engine::Serialize::ContainerPolicy::allowAll, ModuleLoader *, std::string> mInstances;
	Engine::Serialize::Action<Engine::Serialize::ActionPolicy::standard> setupDone;

};

	}
}
