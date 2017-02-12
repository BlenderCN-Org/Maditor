#pragma once

#include "Dialogs\DialogManager.h"
#include "Dialogs\settingsdialog.h"

namespace Maditor {
namespace View {

template <class T>
class ComponentView
{
public:
	ComponentView() :
	mModel(0){

	}

	virtual void setModel(T *model) {
		mModel = model;
		setupConnections();
	}

protected:
	void setConnections(const std::list<std::pair<QAction*, void(T::*)()>> &connections) {
		mConnections = connections;
		if (mModel)
			setupConnections();
	}

private:
	void setupConnections() {
		for (const std::pair<QAction*, void(T::*)()> &p : mConnections) {
			QObject::connect(p.first, &QAction::triggered, mModel, p.second);
		}
	}

protected:
	QToolBar *createToolbar(QMainWindow *window, const QString &name, const QList<QAction *> &actions) {
		QToolBar *toolbar = new QToolBar;
		toolbar->setObjectName(name);

		toolbar->addActions(actions);

		window->addToolBar(toolbar);

		return toolbar;
	}

	void createSettingsTab(Dialogs::DialogManager *dialogs, Dialogs::SettingsTab *tab, const QString &name) {
		dialogs->settingsDialog()->addSettingsTab(tab, name);
	}

	T *model() {
		return mModel;
	}

private:
	std::list<std::pair<QAction *, void (T::*)()>> mConnections;

	T *mModel;
};


} // namespace View
} // namespace Maditor
