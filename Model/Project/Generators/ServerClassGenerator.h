#pragma once

#include "ClassGenerator.h"

namespace Maditor {
	namespace Model {
		namespace Generators {

			class ServerClassGenerator : public ClassGenerator {
			public:
				ServerClassGenerator(Module *module, const QString &name);
				ServerClassGenerator(Module * module, QDomElement data);

				static const QString sType;

			protected:


				// Inherited via ClassGenerator
				virtual QString templateFileName(int index) override;

			};

		}
	}
}