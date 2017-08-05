#include "maditorsharedlib.h"

#include "moduleinstance.h"

#include "Serialize\Streams\serializestream.h"

namespace Maditor {
	namespace Shared {
		ModuleInstance::ModuleInstance(Engine::Serialize::TopLevelSerializableUnitBase *topLevel, const std::string & name) :
			SerializableUnit(topLevel),
			mLoaded(false),
			mExists(false),
			mName(name)/*,
			reload(this)*/
		{
		}

		const std::string & ModuleInstance::name() const
		{
			return mName;
		}

		bool ModuleInstance::exists() const
		{
			return mExists;
		}

		void ModuleInstance::setExists(bool b)
		{
			mExists = b;
		}

		bool ModuleInstance::isLoaded() const
		{
			return mLoaded;
		}
		
		void ModuleInstance::setLoaded(bool b)
		{
			mLoaded = b;
		}

		void ModuleInstance::addDependency(ModuleInstance * dep)
		{
			mDependencies.push_back(dep);
		}

		void ModuleInstance::removeDependency(ModuleInstance * dep)
		{
			mDependencies.remove(dep);
		}
		
		void ModuleInstance::writeCreationData(Engine::Serialize::SerializeOutStream & out) const
		{
			out << mName;
		}

		const Engine::Serialize::ObservableList<ModuleInstance*, Engine::Serialize::ContainerPolicy::allowAll> &ModuleInstance::dependencies()
		{
			return mDependencies;
		}

		void ModuleInstance::reloadImpl()
		{
		}
	}
}