#pragma once

#include "Serialize\serializableunit.h"
#include "Serialize\Container\list.h"
#include "Serialize\Container\action.h"
#include "Serialize\Container\observed.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT ModuleInstanceBase : public Engine::Serialize::SerializableUnitBase {


		private:
			virtual void reloadImpl();
			void reloadImpl2() {
				reloadImpl();
			}

		public:
			ModuleInstanceBase(const std::string &name);

			const std::string &name() const;

			bool exists() const;
			void setExists(bool b);

			bool isLoaded() const;
			void setLoaded(bool b);

			void addDependency(ModuleInstanceBase *dep);
			void removeDependency(ModuleInstanceBase *dep);

			virtual void writeCreationData(Engine::Serialize::SerializeOutStream &out) const override;

			Engine::Serialize::Action<decltype(&ModuleInstanceBase::reloadImpl2), &ModuleInstanceBase::reloadImpl2, Engine::Serialize::ActionPolicy::request> reload;

			const Engine::Serialize::ObservableList<ModuleInstanceBase*, Engine::Serialize::ContainerPolicy::allowAll> &dependencies();

		protected:
			Engine::Serialize::Observed<bool> mLoaded;


		private:
			bool mExists;
			std::string mName;


			Engine::Serialize::ObservableList<ModuleInstanceBase*, Engine::Serialize::ContainerPolicy::allowAll> mDependencies;

		};

		template <class T>
		class ModuleInstance : Engine::Hierarchy::HierarchyObject<T>, public ModuleInstanceBase {
			using ModuleInstanceBase::ModuleInstanceBase;
		};

	}
}