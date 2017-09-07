
#include "maditorlauncherlib.h"

#include "ModuleLoader.h"

#include <iostream>

#include "Scene/Entity/entity.h"
#include "Scene\scenecomponent.h"
#include "Scene/ogrescenemanager.h"

namespace Maditor {
	namespace Launcher {


		ModuleLoader::ModuleLoader(Engine::Serialize::TopLevelSerializableUnitBase *topLevel) :
			SerializableUnit(topLevel),
			mInit(false)
		{
			mInstances.setCreator(std::bind(&ModuleLoader::createModule, this, std::placeholders::_1));
		}

		ModuleLoader::~ModuleLoader()
		{
		}

		void ModuleLoader::setup(const std::string &binaryDir)
		{

			assert(!mInit);
			mInit = true;
			
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
			ModuleInstance(parent->topLevel(), name),
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

		Engine::Server::ServerBase * ModuleLoader::createServer(const std::string & fullName, const std::string &instanceName, const std::string &mediaDir)
		{
			size_t delimPos = fullName.find("::");
			std::string moduleName(fullName.c_str(), delimPos);
			std::string className(fullName.c_str() + (delimPos + 2));

			auto it = std::find_if(mInstances.begin(), mInstances.end(), [&](const ModuleLauncherInstance &mod) {return mod.name() == moduleName; });
			if (it != mInstances.end() && it->isLoaded()) {
				void *symbol = it->getSymbol(std::string("create") + className);
				if (symbol) {
					typedef Engine::Server::ServerBase *(*Factory)(const std::string &, const std::string &);
					return (*static_cast<Factory>(symbol))(instanceName, mediaDir + "scripts/");
				}
			}

			return nullptr;
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

			std::string runtimePath = name() + ".dll";
			std::string runtimePdbPath = name() + ".pdb";
			std::string binaryPath = mParent->binaryDir() + name() + ".dll";
			std::string binaryPdbPath = mParent->binaryDir() + name() + ".pdb";

			std::error_code err;
			if (!std::experimental::filesystem::copy_file(binaryPath, runtimePath, std::experimental::filesystem::copy_options::overwrite_existing, err)) {
				throw 0;
			}

			if (!std::experimental::filesystem::copy_file(binaryPdbPath, runtimePdbPath, std::experimental::filesystem::copy_options::overwrite_existing, err)) {
				std::cout << "Warning: Pdb might not be up to date!" << std::endl;
			}

			mEntityComponentNames.clear();
			mSceneComponentHashes.clear();
			mGameHandlerHashes.clear();
			mGuiHandlerHashes.clear();
			mGlobalAPIComponentHashes.clear();
			mSceneComponents.clear();
			mGameHandlers.clear();
			mGuiHandlers.clear();
			mGlobalAPIComponents.clear();

			Engine::BaseUniqueComponentCollector<Engine::Scene::SceneComponentBase, Engine::Serialize::SerializableUnitHeapCreator, Engine::Scene::SceneManagerBase*> *sceneComponentCollector = Engine::BaseUniqueComponentCollector<Engine::Scene::SceneComponentBase, Engine::Serialize::SerializableUnitHeapCreator, Engine::Scene::SceneManagerBase*>::getSingletonPtr();
			Engine::BaseUniqueComponentCollector<Engine::Scripting::GlobalAPIComponentBase> *globalAPIComponentCollector = Engine::BaseUniqueComponentCollector<Engine::Scripting::GlobalAPIComponentBase>::getSingletonPtr();
			Engine::OgreUniqueComponentCollector<Engine::UI::GameHandlerBase> *gameHandlerCollector = Engine::OgreUniqueComponentCollector<Engine::UI::GameHandlerBase>::getSingletonPtr();
			Engine::OgreUniqueComponentCollector<Engine::UI::GuiHandlerBase> *guiHandlerCollector = Engine::OgreUniqueComponentCollector<Engine::UI::GuiHandlerBase>::getSingletonPtr();

			std::set<std::string> beforeEntityComponents = Engine::Scene::Entity::Entity::registeredComponentNames();
			std::list<void *> beforeSceneComponentsHashesList = sceneComponentCollector->registeredComponentsHashes();
			std::set<void *> beforeSceneComponentsHashes(beforeSceneComponentsHashesList.begin(), beforeSceneComponentsHashesList.end());
			std::list<void *> beforeGlobalAPIComponentsHashesList = globalAPIComponentCollector->registeredComponentsHashes();
			std::set<void *> beforeGlobalAPIComponentsHashes(beforeGlobalAPIComponentsHashesList.begin(), beforeGlobalAPIComponentsHashesList.end());
			std::list<void *> beforeGameHandlerHashesList = gameHandlerCollector->registeredComponentsHashes();
			std::set<void *> beforeGameHandlerHashes(beforeGameHandlerHashesList.begin(), beforeGameHandlerHashesList.end());
			std::list<void *> beforeGuiHandlerHashesList = guiHandlerCollector->registeredComponentsHashes();
			std::set<void *> beforeGuiHandlerHashes(beforeGuiHandlerHashesList.begin(), beforeGuiHandlerHashesList.end());

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
			std::list<void *> afterGameHandlerHashesList = gameHandlerCollector->registeredComponentsHashes();
			std::set<void *> afterGameHandlerHashes(afterGameHandlerHashesList.begin(), afterGameHandlerHashesList.end());
			std::set_difference(afterGameHandlerHashes.begin(), afterGameHandlerHashes.end(), beforeGameHandlerHashes.begin(), beforeGameHandlerHashes.end(), std::inserter(mGameHandlerHashes, mGameHandlerHashes.end()));
			std::list<void *> afterGuiHandlerHashesList = guiHandlerCollector->registeredComponentsHashes();
			std::set<void *> afterGuiHandlerHashes(afterGuiHandlerHashesList.begin(), afterGuiHandlerHashesList.end());
			std::set_difference(afterGuiHandlerHashes.begin(), afterGuiHandlerHashes.end(), beforeGuiHandlerHashes.begin(), beforeGuiHandlerHashes.end(), std::inserter(mGuiHandlerHashes, mGuiHandlerHashes.end()));

			if (sceneComponentCollector) {
				for (void *h : mSceneComponentHashes) {
					auto it = sceneComponentCollector->postCreate(h, Engine::Scene::SceneManagerBase::getSingletonPtr());
					if (callInit)
						(*it)->init();
					mSceneComponents.push_back(it->get());
				}
			}

			if (globalAPIComponentCollector) {
				for (void *h : mGlobalAPIComponentHashes) {
					auto it = globalAPIComponentCollector->postCreate(h);
					if (callInit)
						(*it)->init();
					mGlobalAPIComponents.push_back(it->get());
				}
			}

			if (guiHandlerCollector) {
				for (void *h : mGuiHandlerHashes) {
					auto it = guiHandlerCollector->postCreate(h);
					if (callInit)
						(*it)->init();
					mGuiHandlers.push_back(it->get());
				}
			}

			if (gameHandlerCollector) {
				for (void *h : mGameHandlerHashes) {
					auto it = gameHandlerCollector->postCreate(h);
					if (callInit)
						(*it)->init();
					mGameHandlers.push_back(it->get());
				}
			}

			for (const std::pair<const std::string, std::list<Engine::Scene::Entity::Entity*>> &ents : mStoredComponentEntities) {
				for (Engine::Scene::Entity::Entity*p : ents.second) {
					p->addComponent(ents.first);
				}
			}
			mStoredComponentEntities.clear();

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


			std::list<Engine::Scene::Entity::Entity*> entities;
			if (Engine::Scene::OgreSceneManager *mgr = Engine::Scene::OgreSceneManager::getSingletonPtr())
				entities = mgr->entities();


			for (const std::string &comp : mEntityComponentNames) {
				std::list<Engine::Scene::Entity::Entity*> &componentEntities = mStoredComponentEntities[comp];
				for (Engine::Scene::Entity::Entity* e : entities) {
					if (e->hasComponent(comp)) {
						componentEntities.push_back(e);
						e->removeComponent(comp);
					}
				}
			}

			if (Engine::BaseUniqueComponentCollector<Engine::Scripting::GlobalAPIComponentBase>::getSingletonPtr()) {
				for (Engine::Scripting::GlobalAPIComponentBase *api : mGlobalAPIComponents) {
					if (api->getState() == Engine::ObjectState::INITIALIZED)
						api->finalize();
				}
			}

			if (Engine::BaseUniqueComponentCollector<Engine::Scene::SceneComponentBase, Engine::Serialize::SerializableUnitHeapCreator, Engine::Scene::SceneManagerBase*>::getSingletonPtr()){
				for (Engine::Scene::SceneComponentBase *c : mSceneComponents) {
					if (c->getState() == Engine::ObjectState::INITIALIZED)
						c->finalize();
				}
			}

			if (Engine::OgreUniqueComponentCollector<Engine::UI::GameHandlerBase>::getSingletonPtr()) {
				for (Engine::UI::GameHandlerBase *h : mGameHandlers) {
					if (h->getState() == Engine::ObjectState::INITIALIZED)
						h->finalize();
				}
			}

			if (Engine::OgreUniqueComponentCollector<Engine::UI::GuiHandlerBase>::getSingletonPtr()) {
				for (int i = -1; i < Engine::UI::UIManager::sMaxInitOrder; ++i)
					for (Engine::UI::GuiHandlerBase *h : mGuiHandlers)
						if (h->getState() == Engine::ObjectState::INITIALIZED)
							h->finalize(i);
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
