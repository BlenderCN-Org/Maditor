#pragma once

#include "ui_mainwindow.h"

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
		QObject::connect(ui->tabWidget, &QTabWidget::tabCloseRequested, std::bind(&WindowSpawner<Model, View>::onTabCloseRequestBase, this, std::placeholders::_1));
	}

protected:
	virtual void onTabCloseRequest(View *w) = 0;

	template <class... _Ty>
	void spawn(Model *model, _Ty&&... args) {
		auto it = mViews.find(model);
		if (it == mViews.end()) {
			View *win = new View(model, std::forward<_Ty>(args)...);
			mViews[model] = win;
			mTabWidget->addTab(win, win->windowTitle());
		}
		else {
			mTabWidget->setCurrentWidget(it->second);
		}		
	}

	void remove(Model *model) {
		auto it = mViews.find(model);
		assert(it != mViews.end());
		it->second->deleteLater();
		mViews.erase(it);
	}

	void onTabCloseRequestBase(int index) {
		if (View *w = dynamic_cast<View*>(mTabWidget->widget(index))) {
			onTabCloseRequest(w);
		}
	}


private:
	QTabWidget *mTabWidget;
	
	std::map<Model*, View*> mViews;

};


} // namespace View
} // namespace Maditor
