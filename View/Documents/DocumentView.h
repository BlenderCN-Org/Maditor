#pragma once


namespace Maditor {
	namespace View {

class DocumentView
{
public:
	DocumentView(Model::Document *doc);
	virtual ~DocumentView() = default;	

	Model::Document *document();

protected:


private:
	Model::Document *mDocument;
	
};

	}
}

