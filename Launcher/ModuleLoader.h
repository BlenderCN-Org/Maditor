#pragma once

#include "Shared\moduleinstance.h"
#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Launcher {

class ModuleLoader : public Engine::Serialize::SerializableUnit{

public:
	ModuleLoader(Engine::Serialize::TopLevelSerializableUnit *topLevel);
	~ModuleLoader();

	void setup(const std::string &binaryDir, const std::string &runtimeDir);
	void cleanup();
			
	bool receiving();

private:

	struct ModuleLauncherInstance : public Shared::ModuleInstance {
		ModuleLauncherInstance(const std::string &name) :
			ModuleInstance(name),
			mHandle(0)
		{
		}

		HINSTANCE mHandle;


		std::list<std::string> mEntityComponentNames;
		std::list<Engine::Scene::BaseSceneComponent*> mSceneComponents;
		std::list<Engine::UI::GameHandlerBase*> mGameHandlers;
		std::list<Engine::UI::GuiHandlerBase*> mGuiHandlers;
		std::list<Engine::Scripting::BaseGlobalAPIComponent*> mGlobalAPIComponents;
		std::list<Engine::Scene::SceneListener*> mSceneListeners;
		std::map<std::string, std::list<Engine::Scene::Entity::Entity*>> mStoredComponentEntities;
	};

	
	bool loadModule(ModuleLauncherInstance &module, bool callInit);
	bool unloadModule(ModuleLauncherInstance &module);

	void setupDoneImpl();
	
private:
	std::string mBinaryDir, mRuntimeDir;

	bool mInit;

	bool mReceivingModules;

	Engine::Serialize::ObservableList<ModuleLauncherInstance, std::string> mInstances;
	Engine::Serialize::Action<> setupDone;

};

	}
}
