#include "maditorsharedlib.h"

#include "moduleinstance.h"

#include "Serialize\Streams\serializestream.h"

namespace Maditor {
	namespace Shared {
		ModuleInstanceBase::ModuleInstanceBase(const std::string & name) :
			mLoaded(false),
			mExists(false),
			mName(name),
			mDependencies()
		{
		}

		const std::string & ModuleInstanceBase::name() const
		{
			return mName;
		}

		bool ModuleInstanceBase::exists() const
		{
			return mExists;
		}

		void ModuleInstanceBase::setExists(bool b)
		{
			mExists = b;
		}

		bool ModuleInstanceBase::isLoaded() const
		{
			return mLoaded;
		}
		
		void ModuleInstanceBase::setLoaded(bool b)
		{
			mLoaded = b;
		}

		void ModuleInstanceBase::addDependency(ModuleInstanceBase * dep)
		{
			mDependencies.push_back(dep);
		}

		void ModuleInstanceBase::removeDependency(ModuleInstanceBase * dep)
		{
			mDependencies.remove(dep);
		}
		
		void ModuleInstanceBase::writeCreationData(Engine::Serialize::SerializeOutStream & out) const
		{
			out << mName;
		}

		const Engine::Serialize::ObservableList<ModuleInstanceBase*, Engine::Serialize::ContainerPolicy::allowAll> &ModuleInstanceBase::dependencies()
		{
			return mDependencies;
		}

		void ModuleInstanceBase::reloadImpl()
		{
		}
	}
}