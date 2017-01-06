#include "maditorviewlib.h"

#include "TreeView.h"

#include "Model\TreeModel.h"

namespace Maditor {
	namespace View {


		TreeView::TreeView(QWidget * parent) :
			QTreeView(parent),
			mModel(0)
		{
			connect(this, &QTreeView::customContextMenuRequested, this, &TreeView::buildContextMenu);
			setContextMenuPolicy(Qt::CustomContextMenu);

			if(header()->isSortIndicatorShown())
				header()->setSortIndicator(0, Qt::AscendingOrder);
		}

		TreeView::~TreeView()
		{
		}

		void TreeView::setModel(Model::TreeModel * model)
		{
			if (model) {
				QTreeView::setModel(model->sorted());
				connect(this, &QTreeView::doubleClicked, model->sorted(), &Model::TreeSorter::itemDoubleClicked);
			}
			else {
				QTreeView::setModel(0);
			}

			mModel = model;
		}

		void TreeView::buildContextMenu(const QPoint &p) {
			if (!mModel) return;
			QMenu menu;
			mModel->handleContextMenuRequest(mModel->sorted()->mapToSource(indexAt(p)), menu);
			if (menu.actions().size() > 0)
				menu.exec(viewport()->mapToGlobal(p));
		}

		void TreeView::clearModel()
		{
			setModel(0);
		}		 

	}
}