#include "maditorsharedlib.h"

#include "moduleinstance.h"

#include "Serialize\Streams\serializestream.h"

namespace Maditor {
	namespace Shared {
		ModuleInstance::ModuleInstance(const std::string & name) :
			mExists(false),
			mName(name),
			mLoaded(false)
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
		
		void ModuleInstance::writeCreationData(Engine::Serialize::SerializeOutStream & out) const
		{
			out << mName;
		}
	}
}