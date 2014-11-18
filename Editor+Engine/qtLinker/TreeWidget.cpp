#include "TreeWidget.h"
#include <iostream>
#include <QCoreApplication>
#include <qtconcurrentrun.h>
#include <QThread>
#include "TreeView.h"
TreeWidget::TreeWidget(World *wo, QWidget *parent) :
QWidget(parent)
{
	resize(1280, 960);

	p = parent;
	worldptr = wo;
	w = width();
	h = height();

	f = 0;
	WasHiden = false;
	currentlyused = -1;
	vLayout = new QHBoxLayout(this);

	tree = new TreeWindow(worldptr, this);
	
	view = new QTreeView;
	ob = new ObjectEditor(worldptr);
	ob->setMinimumHeight(h - 75);
	ob->ParentLayout = vLayout;
	//ob->setMinimumWidth(600);
	//view->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));

	view->setModel(tree);
	view->setMinimumHeight(h - 75);
	//view->setMinimumWidth(w - 600);
	vLayout->addWidget(view, 0, Qt::AlignHCenter);
	vLayout->addWidget(ob, 0, Qt::AlignHCenter);

	connect(view,
		SIGNAL(clicked(const QModelIndex&)),
		this,
		SLOT(OnClick(const QModelIndex&)));
}
void TreeWidget::hideEvent(QHideEvent *event)
{
	WasHiden = true;
	if (ob->Activated)
		ob->hide();

}
void TreeWidget::OnClick(const QModelIndex& q)
{
	indexat = q;
	TreeItem *item = tree->getItem(q);
	if (currentlyused != -1)
		ob->hide();
	bool ControlDown = false;
	bool CDown = false;
	bool VDown = false;
	bool XDown = false;
	for (int k = 0; k < evt.size(); k++)
	{
		if (evt.at(k) == Qt::Key::Key_Control)
			ControlDown = true;
		else if (evt.at(k) == Qt::Key::Key_C)
			CDown = true;
		else if (evt.at(k) == Qt::Key::Key_V)
			VDown = true;
		else if (evt.at(k) == Qt::Key::Key_X)
			XDown = true;
	}
	evt.clear();
	if (!ControlDown)
	{

		switch (item->type)
		{
		case TreeType::ADVERTISEMENT:
			LinkA = static_cast<Advertisement*>(item->Link);
			ob->AdvertisementShow(LinkA);
			ob->show();
			ob->MasterChild = true;
			ob->wid = this;
			currentlyused = 0;
			break;
		case TreeType::ENTITY:
			LinkE = static_cast<Entity*>(item->Link);
			ob->EntityShow(LinkE);
			ob->show();
			ob->MasterChild = true;
			ob->wid = this;
			currentlyused = 1;
			break;
		case TreeType::ATTRIBUTE:
			LinkEA = static_cast<EntityAttribute*>(item->Link);
			ob->AttributeShow(LinkEA);
			ob->show();
			ob->MasterChild = true;
			ob->wid = this;
			currentlyused = 2;
			break;
		default:
			break;
		}
		ob->LinkEACopy = this->LinkEACopy;
		ob->LinkECopy = this->LinkECopy;
		ob->LinkACopy = this->LinkACopy;
		ob->LinkCoCopy = this->LinkCoCopy;
	}
	else
	{
		if (CDown)
		{
			switch (item->type)
			{
			case TreeType::ADVERTISEMENT:
				*LinkACopy = *worldptr->ReturnAdvertisementShared(static_cast<Advertisement*>(item->Link)->Name);

				LinkECopy = std::shared_ptr<Entity>(new Entity);
				LinkEACopy = std::shared_ptr<EntityAttribute>(new EntityAttribute);
				LinkCoCopy = std::shared_ptr<Cost>(new Cost);
				break;
			case TreeType::ENTITY:
				*LinkECopy = *worldptr->ReturnEntityShared(static_cast<Entity*>(item->Link)->Name);

				LinkACopy = std::shared_ptr<Advertisement>(new Advertisement);
				LinkEACopy = std::shared_ptr<EntityAttribute>(new EntityAttribute);
				LinkCoCopy = std::shared_ptr<Cost>(new Cost);
				break;
			default:
				break;
			}
		}
		else if (VDown)
		{
			switch (item->type)
			{
			case TreeType::ADVERTISEMENTADD:
				if (LinkACopy != nullptr)
				{
					if (LinkACopy.get()->Name != "Default")
						Copied = true;
					ihw = new InheritanceWidget(new QLineEdit("Name: "));
					connect(ihw->button, SIGNAL(pressed()), SLOT(CreateNew()));
					ihw->show();
					typealloc = 0;
					WhileSync = true;
				}
				break;
			case TreeType::ENTITYADD:
				if (LinkECopy != nullptr) //We actually have a value for LinkE
				{
					if (LinkECopy.get()->Name != "")
						Copied = true;
					ihw = new InheritanceWidget(new QLineEdit("Name: "));
					connect(ihw->button, SIGNAL(pressed()), SLOT(CreateNew()));
					ihw->show();
					typealloc = 1;
					WhileSync = true;
				}
				break;
			default:
				break;
			}
		}
		else if (XDown)
		{
			std::string Link;
			int index;
			switch (item->type)
			{
			case TreeType::ADVERTISEMENT:
				Link = item->data(q.column()).toString().toStdString();
				index = worldptr->AdIndex(Link);
				worldptr->Ads->erase(worldptr->Ads->begin() + index);
				tree->removeRow(q.row(), q.parent());
				DeleteAd(Link);
				worldptr->ExternUpdate = true;
				break;
			case TreeType::ENTITY:
				Link = item->data(q.column()).toString().toStdString();
				index = worldptr->EntityIndex(Link);
				worldptr->Entities->erase(worldptr->Entities->begin() + index);
				tree->removeRow(q.row(), q.parent());
				DeleteEntity(Link);
				worldptr->ExternUpdate = true;
				break;
			default:
				break;
			}
		}
	}
}
TreeWidget::~TreeWidget()
{
	_sleep(10);

}
void clearParentLayout(QLayout *layout)
{
	QLayoutItem *item;
	while ((item = layout->takeAt(0))) {
		if (item->layout()) {
			clearParentLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}

void TreeWidget::paintEvent(QPaintEvent * event)
{
	if (WasHiden)
	{
		WhileSync = false;
		LinkACopy = std::shared_ptr<Advertisement>(new Advertisement);
		LinkECopy = std::shared_ptr<Entity>(new Entity);
		LinkEACopy = std::shared_ptr<EntityAttribute>(new EntityAttribute);
		LinkCoCopy = std::shared_ptr<Cost>(new Cost);
		clearParentLayout(vLayout);

		tree = new TreeWindow(worldptr, this);

		view = new QTreeView;
		ob = new ObjectEditor(worldptr);
		ob->setMinimumHeight(h - 75);
		ob->ParentLayout = vLayout;
		//ob->setMinimumWidth(600);
		//view->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));

		view->setModel(tree);
		view->setMinimumHeight(h - 75);
		//view->setMinimumWidth(w - 600);
		vLayout->addWidget(view, 0, Qt::AlignHCenter);
		vLayout->addWidget(ob, 0, Qt::AlignHCenter);
		WasHiden = false;
		connect(view,
			SIGNAL(clicked(const QModelIndex&)),
			this,
			SLOT(OnClick(const QModelIndex&)));
	}
	if ((f % 1 == 0) && (w != p->width() || h != p->height()))
	{
		float h2 = p->height();
		float w2 = p->width();

		h2 /= h;
		w2 /= w;
		if (h2 == 1 && w2 == 1)
		{
		}
		else{
			if (h2 != 0 && w2 != 0)
			{
				//scroll->setFixedSize(scroll->width() * w2, scroll->height() * h2);
				//mside->setFixedHeight(mside->height() * h2);
				this->setFixedSize(p->width()/* * w2*/ - 90, p->height() - 25 /** h2*/);
			}
		}
		w = p->width();
		h = p->height();
		f = 0;
	};
	f++;
}