#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Launcher {

class ModuleLoader : public Engine::Serialize::SerializableUnit<ModuleLoader> {

public:
	ModuleLoader();
	~ModuleLoader();

	void setup(const std::string &binaryDir, const std::string &runtimeDir);
	void cleanup();
			
	bool receiving();

	const std::string &binaryDir();
	const std::string &runtimeDir();

private:

	class ModuleLauncherInstance : public Shared::ModuleInstance {
	public:
		ModuleLauncherInstance(ModuleLoader *parent, const std::string &name);

		~ModuleLauncherInstance();

		void createDependencies();
		bool load(bool callInit);
		bool unload();

		virtual void reloadImpl() override;

	private:
		HINSTANCE mHandle;

		std::list<ModuleLauncherInstance *> mDependedBy;

		std::list<std::string> mEntityComponentNames;
		std::list<Engine::Scene::SceneComponentBase*> mSceneComponents;
		std::list<Engine::UI::GameHandlerBase*> mGameHandlers;
		std::list<Engine::UI::GuiHandlerBase*> mGuiHandlers;
		std::list<Engine::Scripting::GlobalAPIComponentBase*> mGlobalAPIComponents;
		std::list<void*> mSceneComponentHashes;
		std::list<void*> mGlobalAPIComponentHashes;
		std::list<void*> mGameHandlerHashes;
		std::list<void*> mGuiHandlerHashes;
		std::map<std::string, std::list<std::pair<Engine::Scene::Entity::Entity*, Engine::Scripting::ArgumentList>>> mStoredComponentEntities;

		ModuleLoader *mParent;
	};

	void setupDoneImpl();
	
private:
	std::tuple<ModuleLoader *, std::string> createModule(const std::string &name);

private:
	std::string mBinaryDir, mRuntimeDir;

	bool mInit;

	bool mReceivingModules;

	Engine::Serialize::ObservableList<ModuleLauncherInstance, Engine::Serialize::ContainerPolicy::allowAll, Engine::Serialize::CustomCreator<decltype(&ModuleLoader::createModule)>> mInstances;
	Engine::Serialize::Action<decltype(&ModuleLoader::setupDoneImpl), &ModuleLoader::setupDoneImpl, Engine::Serialize::ActionPolicy::request> setupDone;

};

	}
}
