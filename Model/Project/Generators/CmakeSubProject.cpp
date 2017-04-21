#include "maditormodellib.h"

#include "CmakeSubProject.h"


namespace Maditor {
	namespace Model {
		namespace Generators {

			CmakeSubProject::CmakeSubProject(CmakeGenerator * parent, const QString & name) :
				CmakeGenerator(name),
				mParent(parent)
			{
				
			}

			QString CmakeSubProject::root()
			{
				return mParent->root() + name() + "/";
			}
		}
	}
}