
#include "maditorlauncherlib.h"

#include "ModuleLoader.h"


namespace Maditor {
	namespace Launcher {


		ModuleLoader::ModuleLoader(Engine::Serialize::TopLevelSerializableUnit *topLevel) :
			SerializableUnit(topLevel),
			mInit(false),
			mInstances(this),
			setupDone(this, &ModuleLoader::setupDoneImpl)
		{

		}


		ModuleLoader::~ModuleLoader()
		{
		}

		void ModuleLoader::setup(const std::string &binaryDir, const std::string & runtimeDir)
		{

			assert(!mInit);
			mInit = true;

			mRuntimeDir = runtimeDir;
			mBinaryDir = binaryDir;

			mReceivingModules = true;

		}

		void ModuleLoader::cleanup()
		{

			for (ModuleLauncherInstance &p : mInstances) {
				unloadModule(p);
			}

			mInstances.clear();

			mInit = false;
		}

		bool ModuleLoader::receiving()
		{
			return mReceivingModules;
		}




		bool ModuleLoader::unloadModule(ModuleLauncherInstance & module)
		{

			if (!module.isLoaded())
				return true;

			std::cout << "Unloading " << module.name() << std::endl;

			if (Engine::Scene::SceneManager::getSingleton().isSceneLoaded()) {

				const std::list<Engine::Scene::Entity::Entity*> &entities = Engine::Scene::SceneManager::getSingleton().entities();


				for (const std::string &comp : module.mEntityComponentNames) {
					std::list<Engine::Scene::Entity::Entity*> &componentEntities = module.mStoredComponentEntities[comp];
					for (Engine::Scene::Entity::Entity* e : entities) {
						if (e->hasComponent(comp)) {
							componentEntities.push_back(e);
							e->removeComponent(comp);
						}
					}
				}


				for (Engine::Scene::SceneListener *listener : module.mSceneListeners) {
					listener->beforeSceneClear();
					listener->onSceneClear();
				}
			}

			for (Engine::Scripting::BaseGlobalAPIComponent *api : module.mGlobalAPIComponents) {
				api->finalize();
			}

			for (int i = -1; i < Engine::UI::UIManager::sMaxInitOrder; ++i)
				for (Engine::UI::GuiHandlerBase *h : module.mGuiHandlers)
					h->finalize(i);

			for (Engine::UI::GameHandlerBase *h : module.mGameHandlers) {
				h->finalize();
			}

			bool result = (FreeLibrary(module.mHandle) != 0);
			if (result)
				module.setLoaded(false);
			else
				throw 0;

			return result;
		}


		bool ModuleLoader::loadModule(ModuleLauncherInstance & module, bool callInit)
		{
			if (module.isLoaded())
				return true;

			std::cout << "Loading " << module.name() << std::endl;


			std::string runtimePath = mRuntimeDir + module.name() + ".dll";
			std::string runtimePdbPath = mRuntimeDir + module.name() + ".pdb";
			std::string binaryPath = mBinaryDir + module.name() + ".dll";
			std::string binaryPdbPath = mBinaryDir + module.name() + ".pdb";

			{
				std::ifstream src(binaryPath, std::ios::binary);
				std::ofstream dst(runtimePath, std::ios::binary);

				dst << src.rdbuf();
			}

			module.mEntityComponentNames.clear();
			module.mSceneComponents.clear();
			module.mGameHandlers.clear();
			module.mGuiHandlers.clear();
			module.mGlobalAPIComponents.clear();
			module.mSceneListeners.clear();
			std::set<std::string> beforeEntityComponents = Engine::Scene::Entity::Entity::registeredComponentNames();
			std::set<Engine::Scene::BaseSceneComponent*> beforeSceneComponents = Engine::Scene::SceneManager::getSingleton().getComponents();
			std::set<Engine::UI::GameHandlerBase*> beforeGameHandlers = Engine::UI::UIManager::getSingleton().getGameHandlers();
			std::set<Engine::UI::GuiHandlerBase*> beforeGuiHandlers = Engine::UI::UIManager::getSingleton().getGuiHandlers();
			std::set<Engine::Scripting::BaseGlobalAPIComponent*> beforeAPIComponents = Engine::Scripting::GlobalScope::getSingleton().getGlobalAPIComponents();
			std::set<Engine::Scene::SceneListener*> beforeSceneListeners = Engine::Scene::SceneManager::getSingleton().getListeners();

			UINT errorMode = GetErrorMode();
			SetErrorMode(SEM_FAILCRITICALERRORS);
			try {
				module.mHandle = LoadLibrary(runtimePath.c_str());
			}
			catch (...) {
				module.mHandle = 0;
			}
			SetErrorMode(errorMode);

			if (!module.mHandle)
				return false;

			std::set<std::string> afterEntityComponents = Engine::Scene::Entity::Entity::registeredComponentNames();
			std::set_difference(afterEntityComponents.begin(), afterEntityComponents.end(), beforeEntityComponents.begin(), beforeEntityComponents.end(), std::inserter(module.mEntityComponentNames, module.mEntityComponentNames.end()));
			std::set<Engine::Scene::BaseSceneComponent*> afterSceneComponents = Engine::Scene::SceneManager::getSingleton().getComponents();
			std::set_difference(afterSceneComponents.begin(), afterSceneComponents.end(), beforeSceneComponents.begin(), beforeSceneComponents.end(), std::inserter(module.mSceneComponents, module.mSceneComponents.end()));
			std::set<Engine::UI::GameHandlerBase*> afterGameHandlers = Engine::UI::UIManager::getSingleton().getGameHandlers();
			std::set_difference(afterGameHandlers.begin(), afterGameHandlers.end(), beforeGameHandlers.begin(), beforeGameHandlers.end(), std::inserter(module.mGameHandlers, module.mGameHandlers.end()));
			std::set<Engine::UI::GuiHandlerBase*> afterGuiHandlers = Engine::UI::UIManager::getSingleton().getGuiHandlers();
			std::set_difference(afterGuiHandlers.begin(), afterGuiHandlers.end(), beforeGuiHandlers.begin(), beforeGuiHandlers.end(), std::inserter(module.mGuiHandlers, module.mGuiHandlers.end()));
			std::set<Engine::Scripting::BaseGlobalAPIComponent*> afterAPIComponents = Engine::Scripting::GlobalScope::getSingleton().getGlobalAPIComponents();
			std::set_difference(afterAPIComponents.begin(), afterAPIComponents.end(), beforeAPIComponents.begin(), beforeAPIComponents.end(), std::inserter(module.mGlobalAPIComponents, module.mGlobalAPIComponents.end()));
			std::set<Engine::Scene::SceneListener*> afterSceneListeners = Engine::Scene::SceneManager::getSingleton().getListeners();
			std::set_difference(afterSceneListeners.begin(), afterSceneListeners.end(), beforeSceneListeners.begin(), beforeSceneListeners.end(), std::inserter(module.mSceneListeners, module.mSceneListeners.end()));

			if (callInit) {

				for (int i = 0; i < Engine::UI::UIManager::sMaxInitOrder; ++i)
					for (Engine::UI::GuiHandlerBase *h : module.mGuiHandlers)
						h->init(i);

				for (Engine::UI::GameHandlerBase *h : module.mGameHandlers) {
					h->init();
				}


				for (Engine::Scene::BaseSceneComponent *c : module.mSceneComponents) {
					c->init();
				}

				for (Engine::Scripting::BaseGlobalAPIComponent *api : module.mGlobalAPIComponents) {
					api->init();
				}

				if (Engine::Scene::SceneManager::getSingleton().isSceneLoaded()) {
					for (Engine::Scene::SceneListener *listener : module.mSceneListeners) {
						listener->onSceneLoad();
					}

					for (const std::pair<const std::string, std::list<Engine::Scene::Entity::Entity*>> &ents : module.mStoredComponentEntities) {
						for (Engine::Scene::Entity::Entity* e : ents.second) {
							e->addComponent(ents.first);
						}
					}
					module.mStoredComponentEntities.clear();
				}

			}

			module.setLoaded(true);

			return true;
		}

		void ModuleLoader::setupDoneImpl()
		{
			mReceivingModules = false;
		}

	}
}
