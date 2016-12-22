#include "maditorsharedlib.h"

#include "moduleinstance.h"

#include "Serialize\Streams\serializestream.h"

namespace Maditor {
	namespace Shared {
		ModuleInstance::ModuleInstance(const std::string & name) :
			mExists(false),
			mName(name),
			mLoaded(false),
			mDependencies(this),
			reload(this, &ModuleInstance::reloadImpl)
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

		const Engine::Serialize::ObservableList<ModuleInstance*> &ModuleInstance::dependencies()
		{
			return mDependencies;
		}

		void ModuleInstance::reloadImpl()
		{
		}
	}
}