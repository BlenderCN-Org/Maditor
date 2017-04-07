#pragma once


namespace Maditor {
	namespace Model {

class Document : public QObject
{
	Q_OBJECT
public:
	virtual ~Document() = default;	

	virtual void destroy();

signals:
	void destroySignal(Document *);

protected:


private:
	
};

	}
}

