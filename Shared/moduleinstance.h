#pragma once

#include "Serialize\serializableunit.h"

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

			virtual void writeCreationData(Engine::Serialize::SerializeOutStream &out) const override;

		private:
			bool mExists;
			std::string mName;
			bool mLoaded;

		};

	}
}