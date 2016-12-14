#pragma once

#include "CmakeGenerator.h"

namespace Maditor {
	namespace Model {
		namespace Generators {

			class MADITOR_MODEL_EXPORT CmakeProject : public CmakeGenerator {
			public:
				CmakeProject(const QString &root, const QString &buildDir, const QString &name);

				void build(const QString &buildType);

				// Geerbt �ber CmakeGenerator
				virtual QString root() override;

				void addLibrary(const QString &lib);

				QString solutionName(const QString &buildType);

				QString buildDir(const QString &buildType);

			protected:
				virtual QString preTargetCode() override;

			private:
				QString mBuildDir;
				QString mRoot;

				QStringList mLibraries;

			};

		}
	}
}