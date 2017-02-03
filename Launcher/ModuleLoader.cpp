
#include "maditorlauncherlib.h"

#include "ModuleLoader.h"

#include <iostream>

#include "Scene/Entity/entity.h"
#include "Scene\scenecomponent.h"
#include "Scene/ogrescenemanager.h"

namespace Maditor {
	namespace Launcher {


		ModuleLoader::ModuleLoader() :
			mInit(false),
			mInstances(this, &ModuleLoader::createModule),
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

			mInstances.clear();

			mInit = false;
		}

		bool ModuleLoader::receiving()
		{
			return mReceivingModules;
		}

		const std::string & ModuleLoader::binaryDir()
		{
			return mBinaryDir;
		}

		const std::string & ModuleLoader::runtimeDir()
		{
			return mRuntimeDir;
		}

		void ModuleLoader::setupDoneImpl()
		{
			for (ModuleLauncherInstance &instance : mInstances) {
				instance.createDependencies();
				instance.load(false);
			}
			mReceivingModules = false;
		}

		std::tuple<ModuleLoader*, std::string> ModuleLoader::createModule(const std::string & name)
		{
			return{ this, name };
		}


		ModuleLoader::ModuleLauncherInstance::ModuleLauncherInstance(ModuleLoader * parent, const std::string & name) :
			ModuleInstance(name),
			mHandle(0),
			mParent(parent)
		{

		}

		ModuleLoader::ModuleLauncherInstance::~ModuleLauncherInstance()
		{
			unload();
			for (ModuleInstance *dep : dependencies()) {
				ModuleLauncherInstance *d = dynamic_cast<ModuleLauncherInstance*>(dep);
				if (!d)
					throw 0;
				d->mDependedBy.remove(this);
			}
			for (ModuleLauncherInstance *dep : mDependedBy) {
				dep->removeDependency(this);
			}

		}

		void ModuleLoader::ModuleLauncherInstance::createDependencies()
		{
			for (ModuleInstance *dep : dependencies()) {
				ModuleLauncherInstance *d = dynamic_cast<ModuleLauncherInstance*>(dep);
				if (!d)
					throw 0;
				d->mDependedBy.push_back(this);
			}
		}

		bool ModuleLoader::ModuleLauncherInstance::load(bool callInit)
		{
			if (isLoaded())
				return true;

			for (ModuleInstance *dep : dependencies()) {
				ModuleLauncherInstance *d = dynamic_cast<ModuleLauncherInstance*>(dep);
				if (!d)
					throw 0;
				if (!d->load(callInit))
					return false;
			}

			std::cout << "Loading " << name() << "..." << std::endl;

			CreateDirectory(mParent->runtimeDir().c_str(), NULL);

			std::string runtimePath = mParent->runtimeDir() + name() + ".dll";
			std::string runtimePdbPath = mParent->runtimeDir() + name() + ".pdb";
			std::string binaryPath = mParent->binaryDir() + name() + ".dll";
			std::string binaryPdbPath = mParent->binaryDir() + name() + ".pdb";

			{
				std::ifstream src(binaryPath, std::ios::binary);
				std::ofstream dst(runtimePath, std::ios::binary);

				dst << src.rdbuf();
			}

			mEntityComponentNames.clear();
			mSceneComponents.clear();
			mGameHandlers.clear();
			mGuiHandlers.clear();
			mGlobalAPIComponents.clear();
			std::set<std::string> beforeEntityComponents = Engine::Scene::Entity::Entity::registeredComponentNames();
			std::set<Engine::Scene::BaseSceneComponent*> beforeSceneComponents = Engine::Scene::SceneManager::getSingleton().getComponents();
			std::set<Engine::UI::GameHandlerBase*> beforeGameHandlers = Engine::UI::UIManager::getSingleton().getGameHandlers();
			std::set<Engine::UI::GuiHandlerBase*> beforeGuiHandlers = Engine::UI::UIManager::getSingleton().getGuiHandlers();
			std::set<Engine::Scripting::BaseGlobalAPIComponent*> beforeAPIComponents = Engine::Scripting::GlobalScopeImpl::getSingleton().getGlobalAPIComponents();

			UINT errorMode = GetErrorMode();
			SetErrorMode(SEM_FAILCRITICALERRORS);
			try {
				mHandle = LoadLibrary(runtimePath.c_str());
			}
			catch (const std::exception &e) {
				std::cout << "Load-time error: " << e.what() << std::endl;
				mHandle = 0;
			}
			SetErrorMode(errorMode);

			if (!mHandle) {
				std::cout << "failed!" << std::endl;
				return false;
			}
			std::cout << "success" << std::endl;

			std::set<std::string> afterEntityComponents = Engine::Scene::Entity::Entity::registeredComponentNames();
			std::set_difference(afterEntityComponents.begin(), afterEntityComponents.end(), beforeEntityComponents.begin(), beforeEntityComponents.end(), std::inserter(mEntityComponentNames, mEntityComponentNames.end()));
			std::set<Engine::Scene::BaseSceneComponent*> afterSceneComponents = Engine::Scene::SceneManager::getSingleton().getComponents();
			std::set_difference(afterSceneComponents.begin(), afterSceneComponents.end(), beforeSceneComponents.begin(), beforeSceneComponents.end(), std::inserter(mSceneComponents, mSceneComponents.end()));
			std::set<Engine::UI::GameHandlerBase*> afterGameHandlers = Engine::UI::UIManager::getSingleton().getGameHandlers();
			std::set_difference(afterGameHandlers.begin(), afterGameHandlers.end(), beforeGameHandlers.begin(), beforeGameHandlers.end(), std::inserter(mGameHandlers, mGameHandlers.end()));
			std::set<Engine::UI::GuiHandlerBase*> afterGuiHandlers = Engine::UI::UIManager::getSingleton().getGuiHandlers();
			std::set_difference(afterGuiHandlers.begin(), afterGuiHandlers.end(), beforeGuiHandlers.begin(), beforeGuiHandlers.end(), std::inserter(mGuiHandlers, mGuiHandlers.end()));
			std::set<Engine::Scripting::BaseGlobalAPIComponent*> afterAPIComponents = Engine::Scripting::GlobalScopeImpl::getSingleton().getGlobalAPIComponents();
			std::set_difference(afterAPIComponents.begin(), afterAPIComponents.end(), beforeAPIComponents.begin(), beforeAPIComponents.end(), std::inserter(mGlobalAPIComponents, mGlobalAPIComponents.end()));

			if (callInit) {

				for (int i = 0; i < Engine::UI::UIManager::sMaxInitOrder; ++i)
					for (Engine::UI::GuiHandlerBase *h : mGuiHandlers)
						h->init(i);

				for (Engine::UI::GameHandlerBase *h : mGameHandlers) {
					h->init();
				}


				for (Engine::Scene::BaseSceneComponent *c : mSceneComponents) {
					c->init();
				}

				for (Engine::Scripting::BaseGlobalAPIComponent *api : mGlobalAPIComponents) {
					api->init();
				}

				for (const std::pair<const std::string, std::list<Engine::Scene::Entity::Entity*>> &ents : mStoredComponentEntities) {
					for (Engine::Scene::Entity::Entity* e : ents.second) {
						e->addComponent(ents.first);
					}
				}
				mStoredComponentEntities.clear();

			}

			setLoaded(true);

			return true;
		}

		bool ModuleLoader::ModuleLauncherInstance::unload()
		{
			if (!isLoaded())
				return true;

			for (ModuleLauncherInstance *dep : mDependedBy) {
				if (!dep->unload())
					return false;
			}

			std::cout << "Unloading " << name() << std::endl;


			const std::list<Engine::Scene::Entity::Entity*> &entities = Engine::Scene::OgreSceneManager::getSingleton().entities();


			for (const std::string &comp : mEntityComponentNames) {
				std::list<Engine::Scene::Entity::Entity*> &componentEntities = mStoredComponentEntities[comp];
				for (Engine::Scene::Entity::Entity* e : entities) {
					if (e->hasComponent(comp)) {
						componentEntities.push_back(e);
						e->removeComponent(comp);
					}
				}
			}

			for (Engine::Scripting::BaseGlobalAPIComponent *api : mGlobalAPIComponents) {
				api->finalize();
			}

			for (int i = -1; i < Engine::UI::UIManager::sMaxInitOrder; ++i)
				for (Engine::UI::GuiHandlerBase *h : mGuiHandlers)
					h->finalize(i);

			for (Engine::UI::GameHandlerBase *h : mGameHandlers) {
				h->finalize();
			}

			bool result = (FreeLibrary(mHandle) != 0);
			if (result)
				setLoaded(false);
			else
				throw 0;

			return result;
		}

		void ModuleLoader::ModuleLauncherInstance::reloadImpl()
		{
			if (isLoaded() && unload()) {
				load(true);
			}
		}

	}
}
