#include "maditormodellib.h"

#include "Document.h"

namespace Maditor {
	namespace Model {



		void Document::destroy()
		{
			emit destroySignal(this);
		}

	}
}
