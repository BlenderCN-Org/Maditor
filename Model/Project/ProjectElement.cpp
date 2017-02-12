#include "maditormodellib.h"

#include "ProjectElement.h"

#include "Project.h"

namespace Maditor {
	namespace Model {

		ProjectElement::ProjectElement(const QString & name, const QString &type, ProjectElement *parent) :
			mRootElement(parent->document().createElement(type)),
			mName(name),
			mParent(parent)
		{
			mRootElement.setAttribute("name", mName);
			parent->mRootElement.appendChild(mRootElement);
		}
		ProjectElement::ProjectElement(const QString & name, const QString & type, QDomDocument &doc) :
			mRootElement(doc.createElement(type)),
			mName(name),
			mParent(0)
		{
			mRootElement.setAttribute("name", mName);
			doc.appendChild(mRootElement);
		}
		ProjectElement::ProjectElement(QDomElement data, ProjectElement * parent) :
			mRootElement(data),
			mName(data.attribute("name")),
			mParent(parent)
		{
		}

		ProjectElement::ProjectElement(const QString & name, ProjectElement * parent) :
			mName(name),
			mParent(parent)
		{
		}

		ProjectElement::~ProjectElement()
		{
			if (mParent)
				mParent->mRootElement.removeChild(mRootElement);
		}

		const QString &ProjectElement::name() const
		{
			return mName;
		}

		QModelIndex ProjectElement::ownIndex()
		{
			return parentItem() == 0 ? QModelIndex() : project()->index(parentIndex(), 0, parentItem()->ownIndex());
		}

		QString ProjectElement::type() const
		{
			return mRootElement.attribute("type");
		}

		QDomDocument ProjectElement::document()
		{
			return mRootElement.ownerDocument();
		}
		QDomElement ProjectElement::element()
		{
			return mRootElement;
		}

		int ProjectElement::intAttribute(const QString & name)
		{
			return std::stoi(mRootElement.attribute(name).toStdString());
		}

		bool ProjectElement::boolAttribute(const QString & name)
		{
			return mRootElement.attribute(name) != "0";
		}


		ProjectElement * ProjectElement::parentItem()
		{
			return mParent;
		}

		QVariant ProjectElement::data(int col) const
		{
			return mName;
		}



	}
}