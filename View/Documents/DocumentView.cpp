#include "maditorviewlib.h"

#include "DocumentView.h"

namespace Maditor {
	namespace View {
		DocumentView::DocumentView(Model::Document * doc) :
			mDocument(doc)
		{
		}
		Model::Document * DocumentView::document()
		{
			return mDocument;
		}
	}
}
