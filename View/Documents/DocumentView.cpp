#include "maditorviewlib.h"

#include "DocumentView.h"
#include "Model\Documents\Document.h"

namespace Maditor {
	namespace View {
		DocumentView::DocumentView(Model::Document * doc, QWidget *widget) :
			mDocument(doc),
			mWidget(widget),
			mSaveShortcut(QKeySequence(QObject::tr("Ctrl+S", "Save")), widget)
		{
			mConnection = QObject::connect(doc, &Model::Document::dirtyFlagChanged, [=](bool flag) {updateTitle(); });
			QObject::connect(&mSaveShortcut, &QShortcut::activated, doc, &Model::Document::save);
			updateTitle();
		}
		DocumentView::~DocumentView()
		{
			QObject::disconnect(mConnection);
		}
		Model::Document * DocumentView::document()
		{
			return mDocument;
		}
		void DocumentView::updateTitle()
		{
			mWidget->setWindowTitle(mDocument->getQualifiedName());
		}
		bool DocumentView::requestClose()
		{
			if (mDocument->getDirtyFlag()) {
				QMessageBox::StandardButton answer = 
					QMessageBox::question(NULL, "Unsaved Changes", QString("There are unsaved changes in <i>%1</i>! Save?").arg(mDocument->getName()), 
						QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
				switch (answer) {
				case QMessageBox::Abort:
					return false;
				case QMessageBox::Yes:
					mDocument->save();
					break;
				case QMessageBox::No:
					mDocument->discardChanges();
					break;
				}
			}
			return true;
		}
	}
}
