#pragma once

#include "mainwindow_ui.h"
#include "DocumentView.h"

namespace Maditor{
	namespace View{



template <class Model, class View>
class WindowSpawner
{
public:
	WindowSpawner() :
		mTabWidget(nullptr)
	{
	}

	void setupUi(Ui::MainWindow * ui) {
		mTabWidget = ui->tabWidget;
		QObject::connect(ui->tabWidget, &QTabWidget::tabCloseRequested, std::bind(&WindowSpawner<Model, View>::tabCloseAction, this, std::placeholders::_1));
		QObject::connect(ui->tabWidget, &QTabWidget::currentChanged, std::bind(&WindowSpawner<Model, View>::currentTabChangedAction, this, std::placeholders::_1));
	}

	template <class... _Ty>
	void spawn(Model *model, _Ty&&... args) {
		assert(mTabWidget);
		auto it = mViews.find(model);
		if (it == mViews.end()) {
			View *win = new View(model, std::forward<_Ty>(args)...);
			mViews[model] = win;
			win->updateTitle();
			mTabWidget->setCurrentIndex(mTabWidget->addTab(win, win->windowTitle()));
			QObject::connect(win, &View::windowTitleChanged, [=](const QString &title) {mTabWidget->setTabText(mTabWidget->indexOf(win), title); });
		}
		else {
			mTabWidget->setCurrentWidget(it->second);
		}
	}

protected:
	
	void remove(Model *model) {
		auto it = mViews.find(model);
		assert(it != mViews.end());
		it->second->deleteLater();
		mViews.erase(it);
	}

	void tabCloseAction(int index) {
		if (View *w = dynamic_cast<View*>(mTabWidget->widget(index))) {
			if (std::find_if(mViews.begin(), mViews.end(), [=](const std::pair<Model * const, View *> &p) {return p.second == w; }) != mViews.end())
				if (w->requestClose(false) != QMessageBox::Abort)
					w->document()->destroy();
		}
	}

	void currentTabChangedAction(int index) {
		if (View *w = dynamic_cast<View*>(mTabWidget->widget(index))) {
			if (std::find_if(mViews.begin(), mViews.end(), [=](const std::pair<Model * const, View *> &p) {return p.second == w; }) != mViews.end()) {
				currentTabSet(w);
			}
		}
		currentTabCleared(mTabWidget->widget(index));
	}

	virtual void currentTabSet(View *win) {}
	virtual void currentTabCleared(QWidget *w) {}

private:
	QTabWidget *mTabWidget;
	
	std::map<Model*, View*> mViews;

};


} // namespace View
} // namespace Maditor
