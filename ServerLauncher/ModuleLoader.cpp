
#include "maditorserverlauncherlib.h"

#include "ModuleLoader.h"

#include <iostream>

#include "Scene/Entity/entity.h"
#include "Scene\scenecomponent.h"

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

		Engine::Server::BaseServer * ModuleLoader::createServer(const std::string & fullName)
		{
			size_t delimPos = fullName.find(':');
			std::string moduleName(fullName.c_str(), delimPos);
			std::string className(fullName.c_str() + (delimPos + 1));

			auto it = std::find_if(mInstances.begin(), mInstances.end(), [&](const ModuleLauncherInstance &mod) {return mod.name() == moduleName; });
			if (it != mInstances.end() && it->isLoaded()) {
				void *symbol = it->getSymbol(std::string("create") + className);
				if (symbol) {
					typedef Engine::Server::BaseServer *(*Factory)();					
					return (*static_cast<Factory>(symbol))();
				}
			}

			return nullptr;
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
			mSceneComponentHashes.clear();
			mGlobalAPIComponentHashes.clear();

			Engine::UniqueComponentCollector<Engine::Scene::BaseSceneComponent> *sceneComponentCollector = Engine::UniqueComponentCollector<Engine::Scene::BaseSceneComponent>::getSingletonPtr();
			Engine::UniqueComponentCollector<Engine::Scripting::BaseGlobalAPIComponent> *globalAPIComponentCollector = Engine::UniqueComponentCollector<Engine::Scripting::BaseGlobalAPIComponent>::getSingletonPtr();

			std::set<std::string> beforeEntityComponents = Engine::Scene::Entity::Entity::registeredComponentNames();
			std::list<void *> beforeSceneComponentsHashesList = sceneComponentCollector->registeredComponentsHashes();
			std::set<void *> beforeSceneComponentsHashes(beforeSceneComponentsHashesList.begin(), beforeSceneComponentsHashesList.end());
			std::list<void *> beforeGlobalAPIComponentsHashesList = globalAPIComponentCollector->registeredComponentsHashes();
			std::set<void *> beforeGlobalAPIComponentsHashes(beforeGlobalAPIComponentsHashesList.begin(), beforeGlobalAPIComponentsHashesList.end());

			UINT errorMode = GetErrorMode();
			//SetErrorMode(SEM_FAILCRITICALERRORS);
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
			std::list<void *> afterSceneComponentsHashesList = sceneComponentCollector->registeredComponentsHashes();
			std::set<void *> afterSceneComponentsHashes(afterSceneComponentsHashesList.begin(), afterSceneComponentsHashesList.end());
			std::set_difference(afterSceneComponentsHashes.begin(), afterSceneComponentsHashes.end(), beforeSceneComponentsHashes.begin(), beforeSceneComponentsHashes.end(), std::inserter(mSceneComponentHashes, mSceneComponentHashes.end()));
			std::list<void *> afterGlobalAPIComponentsHashesList = globalAPIComponentCollector->registeredComponentsHashes();
			std::set<void *> afterGlobalAPIComponentsHashes(afterGlobalAPIComponentsHashesList.begin(), afterGlobalAPIComponentsHashesList.end());
			std::set_difference(afterGlobalAPIComponentsHashes.begin(), afterGlobalAPIComponentsHashes.end(), beforeGlobalAPIComponentsHashes.begin(), beforeGlobalAPIComponentsHashes.end(), std::inserter(mGlobalAPIComponentHashes, mGlobalAPIComponentHashes.end()));

			if (callInit) {

				if (sceneComponentCollector) {
					for (void *h : mSceneComponentHashes) {
						auto it = sceneComponentCollector->begin();
						std::advance(it, std::distance(afterSceneComponentsHashesList.begin(), std::find(afterSceneComponentsHashesList.begin(), afterSceneComponentsHashesList.end(), h)));
						(*it)->init();
					}
				}

				if (globalAPIComponentCollector) {
					for (void *h : mSceneComponentHashes) {
						auto it = globalAPIComponentCollector->begin();
						std::advance(it, std::distance(afterGlobalAPIComponentsHashesList.begin(), std::find(afterGlobalAPIComponentsHashesList.begin(), afterGlobalAPIComponentsHashesList.end(), h)));
						(*it)->init();
					}
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


			const std::list<Engine::Scene::Entity::Entity*> &entities = Engine::Scene::SceneManager::getSingletonPtr() ? Engine::Scene::SceneManager::getSingleton().entities() : std::list<Engine::Scene::Entity::Entity*>();


			for (const std::string &comp : mEntityComponentNames) {
				std::list<Engine::Scene::Entity::Entity*> &componentEntities = mStoredComponentEntities[comp];
				for (Engine::Scene::Entity::Entity* e : entities) {
					if (e->hasComponent(comp)) {
						componentEntities.push_back(e);
						e->removeComponent(comp);
					}
				}
			}

			Engine::UniqueComponentCollector<Engine::Scripting::BaseGlobalAPIComponent> *globalAPIComponentCollector = Engine::UniqueComponentCollector<Engine::Scripting::BaseGlobalAPIComponent>::getSingletonPtr();
			if (globalAPIComponentCollector) {
				std::list<void*> globalAPIComponentsHashesList = globalAPIComponentCollector->registeredComponentsHashes();
				for (void *h : mSceneComponentHashes) {
					auto it = globalAPIComponentCollector->begin();
					std::advance(it, std::distance(globalAPIComponentsHashesList.begin(), std::find(globalAPIComponentsHashesList.begin(), globalAPIComponentsHashesList.end(), h)));
					(*it)->finalize();
				}
			}

			Engine::UniqueComponentCollector<Engine::Scene::BaseSceneComponent> *sceneComponentCollector = Engine::UniqueComponentCollector<Engine::Scene::BaseSceneComponent>::getSingletonPtr();
			if (sceneComponentCollector) {
				std::list<void*> sceneComponentsHashesList = sceneComponentCollector->registeredComponentsHashes();
				for (void *h : mSceneComponentHashes) {
					auto it = sceneComponentCollector->begin();
					std::advance(it, std::distance(sceneComponentsHashesList.begin(), std::find(sceneComponentsHashesList.begin(), sceneComponentsHashesList.end(), h)));
					(*it)->finalize();
				}
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

		void * ModuleLoader::ModuleLauncherInstance::getSymbol(const std::string symbolName)
		{
			return GetProcAddress(mHandle, symbolName.c_str());
		}

	}
}
