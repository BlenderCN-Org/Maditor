#pragma once


namespace Maditor {
	namespace View {

class DocumentView
{
public:
	DocumentView(Model::Document *doc, QWidget *widget);
	virtual ~DocumentView();	

	Model::Document *document();

	void updateTitle();

	virtual bool requestClose();

private:
	Model::Document *mDocument;
	QWidget *mWidget;
	QMetaObject::Connection mConnection;

	QShortcut mSaveShortcut;
	
};

	}
}

