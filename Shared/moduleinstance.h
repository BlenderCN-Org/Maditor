#pragma once

#include "Serialize\serializableunit.h"
#include "Serialize\Container\list.h"
#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT ModuleInstance : public Engine::Serialize::SerializableUnit {

		public:
			ModuleInstance(const std::string &name);

			const std::string &name() const;

			bool exists() const;
			void setExists(bool b);

			bool isLoaded() const;
			void setLoaded(bool b);

			void addDependency(ModuleInstance *dep);
			void removeDependency(ModuleInstance *dep);

			virtual void writeCreationData(Engine::Serialize::SerializeOutStream &out) const override;

			Engine::Serialize::Action<> reload;

			const Engine::Serialize::ObservableList<ModuleInstance*> &dependencies();


		private:
			virtual void reloadImpl();

		private:
			bool mExists;
			std::string mName;
			bool mLoaded;

			Engine::Serialize::ObservableList<ModuleInstance*> mDependencies;

		};

	}
}