#pragma once


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
		for (const std::list<std::pair<QAction*, void(T::*)()>> &toolbar : mToolbars) {
			setupToolbar(toolbar);
		}
	}

private:
	void setupToolbar(const std::list<std::pair<QAction*, void(T::*)()>> &toolbar) {
		for (const std::pair<QAction*, void(T::*)()> &p : toolbar) {
			QObject::connect(p.first, &QAction::triggered, mModel, p.second);
		}
	}

protected:
	void createToolbar(QMainWindow *window, const QString &name, const std::list<std::pair<QAction *, void (T::*)()>> &actions) {
		QToolBar *toolbar = new QToolBar;
		toolbar->setObjectName(name);

		QList<QAction*> qactions;
		for (const std::pair<QAction*, void (T::*)()> &p : actions) {
			qactions.push_back(p.first);
		}

		toolbar->addActions(qactions);

		window->addToolBar(toolbar);

		mToolbars.push_back(actions);

		if (mModel)
			setupToolbar(actions);
	}

	T *model() {
		return mModel;
	}

private:
	std::list<std::list<std::pair<QAction *, void (T::*)()>>> mToolbars;

	T *mModel;
};


} // namespace View
} // namespace Maditor
