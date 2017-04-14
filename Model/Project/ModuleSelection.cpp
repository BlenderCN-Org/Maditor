#include "maditormodellib.h"

#include "ModuleSelection.h"

#include "ModuleList.h"

#include "ApplicationConfig.h"

#include "Project.h"

namespace Maditor {
	namespace Model {

		ModuleSelection::ModuleSelection(ApplicationConfig *parent) :
			ProjectElement("ModuleSelection", parent),
			mModules(parent->project()->moduleList()),
			mParent(parent)
		{

		}

		ModuleSelection::ModuleSelection(QDomElement element, ApplicationConfig * parent) :
			ProjectElement(element, parent),
			mModules(parent->project()->moduleList()),
			mParent(parent)
		{
		}

		Q_INVOKABLE int ModuleSelection::rowCount(const QModelIndex & parent) const
		{
			return mModules->childCount();
		}

		Q_INVOKABLE QVariant ModuleSelection::data(const QModelIndex & index, int role) const
		{
			if (!index.isValid())
				return QVariant();

			QString name = mModules->child(index.row())->name();
			switch (role) {
			case Qt::CheckStateRole:
				return std::find(mExcludedModules.begin(), mExcludedModules.end(), name) == mExcludedModules.end() ? Qt::Checked : Qt::Unchecked;
			case Qt::DisplayRole:
				return name;
			default:
				return QVariant();
			}
			
		}

		void ModuleSelection::itemClicked(const QModelIndex & index)
		{
			QString name = mModules->child(index.row())->name();
			auto it = std::find(mExcludedModules.begin(), mExcludedModules.end(), name);
			if (it == mExcludedModules.end()) {
				mExcludedModules.push_back(name);
			}
			else {
				mExcludedModules.erase(it);
			}
			emit dataChanged(index, index, { Qt::CheckStateRole });
			mParent->setDirtyFlag(true);
		}

		bool ModuleSelection::storeData()
		{
			for (QDomElement el = element().firstChildElement("exclude"); !el.isNull(); el = element().firstChildElement("exclude")) {
				element().removeChild(el);
			}
			for (const QString &name : mExcludedModules) {
				QDomElement el = element().ownerDocument().createElement("exclude");
				el.appendChild(element().ownerDocument().createTextNode(name));
				element().appendChild(el);
			}
			return true;
		}

		void ModuleSelection::restoreData()
		{
			mExcludedModules.clear();
			for (QDomElement el = element().firstChildElement("exclude"); !el.isNull(); el = el.nextSiblingElement("exclude")) {
				mExcludedModules.push_back(el.text());
			}
		}

	}
}