#pragma once


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

	struct ModuleInstance {
		ModuleInstance(const std::string &name) :
			mHandle(0),
			mLoaded(false),
			mName(name) {}

		HINSTANCE mHandle;
		bool mLoaded;
		std::string mName;

		std::list<std::string> mEntityComponentNames;
		std::list<Engine::Scene::BaseSceneComponent*> mSceneComponents;
		std::list<Engine::UI::GameHandlerBase*> mGameHandlers;
		std::list<Engine::UI::GuiHandlerBase*> mGuiHandlers;
		std::list<Engine::Scripting::BaseGlobalAPIComponent*> mGlobalAPIComponents;
		std::list<Engine::Scene::SceneListener*> mSceneListeners;
		std::map<std::string, std::list<Engine::Scene::Entity::Entity*>> mStoredComponentEntities;
	};

	ModuleInstance &addModule(const std::string &name);

	bool loadModule(ModuleInstance &module, bool callInit);
	bool unloadModule(ModuleInstance &module);

	std::map<std::string, ModuleInstance> mInstances;

	std::string mBinaryDir, mRuntimeDir;

	std::list<std::string> mReloadOrder;
	//std::mutex mReloadMutex;

	bool mInit;

	bool mReceivingModules;


};

	}
}
