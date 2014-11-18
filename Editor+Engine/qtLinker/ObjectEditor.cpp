#include "TreeWidget.h"
#include <iostream>
#include <QCoreApplication>
#include <qtconcurrentrun.h>
#include <QThread>
#include "ObjectEditor.h"
#include "GradEditor.h"
#include "TreeWidget.h"
ObjectEditor::ObjectEditor(World *wo, QWidget *parent) :
QWidget(parent)
{
	resize(1280, 960);
	p = parent;
	worldptr = wo;
	w = width();
	h = height();
	//setFixedHeight(900);
	f = 0;
	last = -1;
	vLayout = new QVBoxLayout(this);
	Activated = true;
	ob = new ObjectEditor;
	MasterChild = false;
}
ObjectEditor::ObjectEditor(QWidget *parent) :
QWidget(parent)
{
	MasterChild = false;
	Activated = false;
}
#ifndef CLEAR_LAYOUT_FUNC
#define CLEAR_LAYOUT_FUNC
void clearLayout(QLayout *layout)
{
	QLayoutItem *item;
	while ((item = layout->takeAt(0))) {
		if (item->layout()) {
			clearLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}
#endif
ObjectEditor::~ObjectEditor()
{
	_sleep(10);
};
void ObjectEditor::EntityShow(Entity *in)
{
	clearLayout(vLayout);
	Clear();
	//newTB("Name", in->Name.c_str());
	tree = new TreeWindowEntity(in, this);
	view = new QTreeView(this);
	e = in;
	connect(view,
		SIGNAL(clicked(const QModelIndex&)),
		this,
		SLOT(OnClick(const QModelIndex&)));
	view->setModel(tree);
	vLayout->addWidget(view, 0, Qt::AlignHCenter);
	view->show();
};

void ObjectEditor::GradShow(Gradient *in)
{
	QTabWidget *tabs = new QTabWidget(this);
	GradientEditor2DAdvanced *gradeditor = new GradientEditor2DAdvanced(worldptr, in, this);
	tabs->addTab(gradeditor, "2D");
	vLayout->addWidget(tabs);
	InUse = 3;
};
void ObjectEditor::CostShow(Cost *in)
{
	newTB("Type", (void*)&in->StringT, Link::STRING);
	newTB("Value", (void*)&std::to_string(in->DiminishingReturn), Link::INT);
	m_button = new QPushButton("Update");
	vLayout->addWidget(m_button, 0, Qt::AlignBottom);
	connect(m_button, SIGNAL(clicked()), this, SLOT(OnButton()));

	update();
	this->co = in;
	InUse = 2;
};
void ObjectEditor::AttributeShow(EntityAttribute *in)
{
	newTB("Type", (void*)&in->Name, Link::STRING);
	newTB("Value", (void*)&std::to_string(in->Value), Link::INT);
	newTB("Delta", (void*)&std::to_string(in->Delta), Link::INT);
	tree = new TreeWindowEntity(in, this);
	view = new QTreeView(this);
	view->setModel(tree);
	vLayout->addWidget(view, 0, Qt::AlignHCenter);
	view->show();
	m_button = new QPushButton("Update");
	vLayout->addWidget(m_button, 0, Qt::AlignBottom);

	connect(view,
		SIGNAL(clicked(const QModelIndex&)),
		this,
		SLOT(OnClick(const QModelIndex&)));
	connect(m_button, SIGNAL(clicked()), this, SLOT(OnButton()));
	update();
	this->ea = in;
	InUse = 1;
};
void ObjectEditor::AdvertisementShow(Advertisement *in)
{
	clearLayout(vLayout);
	Clear();
	newTB("Time Constant", (void*)&std::to_string(in->TimeConstant), Link::INT);
	newTB("Poly-Usability", (void*)&std::to_string(in->UsableByMoreThanOne), Link::BOOL);
	Advertisement temp = *in;
	if (temp.OwnerSet == true)
		newTB("Owner", (void*)&in->GetOwner()->Name, Link::STRING);
	else
		newTB("Owner", QString::fromStdString("NULL"), Link::STRING);

	tree = new TreeWindowEntity(in, this);
	view = new QTreeView(this);
	view->setModel(tree);
	vLayout->addWidget(view, 0, Qt::AlignHCenter);

	connect(view,
		SIGNAL(clicked(const QModelIndex&)),
		this,
		SLOT(OnClick(const QModelIndex&)));
	view->show();

	m_button = new QPushButton("Update");
	vLayout->addWidget(m_button, 0, Qt::AlignBottom);

	connect(m_button, SIGNAL(clicked()), this, SLOT(OnButton()));
	update();
	this->ad = in;
	InUse = 0;
};
void ObjectEditor::hideEvent(QHideEvent *event)
{

	if (ob->Activated)
		ob->hide();

}
void ObjectEditor::OnButton()
{
	switch (InUse)
	{
	case 0: //Advertisement
		ad->TimeConstant = textboxes[0].txt->Title2->text().toInt();
		if (textboxes[1].txt->Title2->text().toInt() == 1)
			ad->UsableByMoreThanOne = true;
		else
			ad->UsableByMoreThanOne = false;
		if (textboxes[2].txt->Title2->text() != "NULL")
			ad->SetOwner(worldptr->ReturnEntity(textboxes[2].txt->Title2->text().toStdString()));
		update();
		break;
	case 1: //Attribute
		ea->Name = textboxes[0].txt->Title2->text().toStdString();
		ea->Value = textboxes[1].txt->Title2->text().toInt();
		ea->Delta = textboxes[2].txt->Title2->text().toInt();
		break;
	case 2: //Cost
		co->StringT = textboxes[0].txt->Title2->text().toStdString();
		co->DiminishingReturn = textboxes[1].txt->Title2->text().toInt();

		update();
		break;
	default:
		break;
	}
}
void ObjectEditor::OnClick(const QModelIndex& q)
{
	TreeItem *item = tree->getItem(q);

	if (last != -1)
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
			last = 0;
			ob = new ObjectEditor(this->worldptr, p);
			ob->ParentLayout = ParentLayout;
			ob->MasterChild = false;
			ob->parentob = this;
			ob->wid = wid;
			ob->AdvertisementShow(static_cast<Advertisement*>(item->Link));
			ParentLayout->addWidget(ob, 0, Qt::AlignHCenter);
			break;
		case TreeType::ENTITY:
			last = 1;
			ob = new ObjectEditor(this->worldptr, p);
			ob->ParentLayout = ParentLayout;
			ob->parentob = this;
			ob->MasterChild = false;
			ob->wid = wid;
			ob->EntityShow(static_cast<Entity*>(item->Link));
			ParentLayout->addWidget(ob, 0, Qt::AlignHCenter);
			break;
		case TreeType::COST:
			ob = new ObjectEditor(this->worldptr, p);
			ob->ParentLayout = ParentLayout;
			ob->parentob = this;
			ob->MasterChild = false;
			ob->wid = wid;
			ob->CostShow(static_cast<Cost*>(item->Link));
			ParentLayout->addWidget(ob, 0, Qt::AlignHCenter);
			last = 4;
			break;
		case TreeType::ATTRIBUTE:
			ob = new ObjectEditor(this->worldptr, p);
			ob->ParentLayout = ParentLayout;
			ob->parentob = this;
			ob->MasterChild = false;
			ob->wid = wid;
			ob->AttributeShow(static_cast<EntityAttribute*>(item->Link));
			ParentLayout->addWidget(ob, 0, Qt::AlignHCenter);
			last = 4;
			break;
		case TreeType::GRAD:
			ob = new ObjectEditor(this->worldptr, p);
			ob->ParentLayout = ParentLayout;
			ob->parentob = this;
			ob->MasterChild = false;
			ob->wid = wid;
			ob->GradShow(&ea->grad);
			ParentLayout->addWidget(ob, 0, Qt::AlignHCenter);
			last = 5;
			break;
		default:
			break;
		}
		ob->LinkECopy = LinkECopy;
		ob->LinkEACopy = LinkEACopy;
		ob->LinkCoCopy = LinkCoCopy;
		ob->LinkACopy = LinkACopy;
	}
	else
	{
		if (CDown)
		{
			Advertisement *at;
			switch (item->type)
			{
			case TreeType::ADVERTISEMENT:
				*LinkACopy = *static_cast<Advertisement*>(item->Link);
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
			case TreeType::ATTRIBUTE:
				*LinkEACopy = *static_cast<EntityAttribute*>(item->Link);

				LinkACopy = std::shared_ptr<Advertisement>(new Advertisement);
				LinkECopy = std::shared_ptr<Entity>(new Entity);
				LinkCoCopy = std::shared_ptr<Cost>(new Cost);
				break;
			case TreeType::COST:
				*LinkCoCopy = *static_cast<Cost*>(item->Link);

				LinkACopy = std::shared_ptr<Advertisement>(new Advertisement);
				LinkECopy = std::shared_ptr<Entity>(new Entity);
				LinkEACopy = std::shared_ptr<EntityAttribute>(new EntityAttribute);
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
					this->indexof = q;
					wid->ihw = new InheritanceWidget(new QLineEdit("Name: "));
					connect(wid->ihw->button, SIGNAL(pressed()), SLOT(CreateNew()));
					wid->ihw->show();
					typealloc = 0;
					wid->WhileSync = true;
				}
				break;
			case TreeType::ENTITYADD:
				if (LinkECopy->Name != "") //We actually have a value for LinkE
				{
					this->indexof = q;
					wid->ihw = new InheritanceWidget(new QLineEdit("Name: "));
					connect(wid->ihw->button, SIGNAL(pressed()), SLOT(CreateNew()));
					wid->ihw->show();
					typealloc = 1;
					wid->WhileSync = true;
				}
				break;
			case TreeType::ATTRIBUTEADD:
				if (LinkEACopy != nullptr) //We actually have a value for LinkE
				{
					this->indexof = q;
					wid->ihw = new InheritanceWidget(new QLineEdit("Name: "));
					connect(wid->ihw->button, SIGNAL(pressed()), SLOT(CreateNew()));
					wid->ihw->show();
					typealloc = 2;
					wid->WhileSync = true;
				}
				break;
			case TreeType::COSTADD:
				if (this->LinkCoCopy != nullptr) //We actually have a value for LinkE
				{
					this->indexof = q;
					wid->ihw = new InheritanceWidget(new QLineEdit("Name: "));
					connect(wid->ihw->button, SIGNAL(pressed()), SLOT(CreateNew()));
					wid->ihw->show();
					typealloc = 3;
					wid->WhileSync = true;
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
			bool check = true;
			switch (item->type)
			{
			case TreeType::ADVERTISEMENT:
				Link = item->data(q.column()).toString().toStdString();
				if (ad != nullptr)
				{
					if (ad->Infer != nullptr)
					{
						if (ad->Infer->Name == Link)
						{
							ad->Infer = nullptr;
							check = false;
						}
					}
				}
			
			if (check)
			if (worldptr->DeleteEntityAdvertisement(Link, e) == 0) //Wasn't found
			{
				//report error
			}
			tree->removeRow(q.row(), q.parent());
			break;
		case TreeType::COST:
			Link = item->data(q.column()).toString().toStdString();
			for (int i = 0; i < ad->positive_effects.size(); i++)
			{
				if (ad->positive_effects.at(i).StringT == Link)
				{
					ad->positive_effects.erase(ad->positive_effects.begin() + i);
					check = false;
					break;
				}
			}
			if (check)
			{
				for (int i = 0; i < ad->negative_effects.size(); i++)
				{
					if (ad->negative_effects.at(i).StringT == Link)
					{
						ad->negative_effects.erase(ad->negative_effects.begin() + i);
						break;
					}
				}
			}
			tree->removeRow(q.row(), q.parent());
			break;
		case TreeType::ATTRIBUTE:
			e->DeleteAttribute(item->data(q.column()).toString().toStdString());
			if (!worldptr->AttributeExist(item->data(q.column()).toString().toStdString()))
				worldptr->Attributes->erase(worldptr->Attributes->begin() + worldptr->AttributeIndex(item->data(q.column()).toString().toStdString()));
			tree->removeRow(q.row(), q.parent());
			break;
		default:
			break;
		}
	}
}
}
void ObjectEditor::paintEvent(QPaintEvent * event)
{
	if (p)
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
void ObjectEditor::CreateNew()
{
	QPointF point;
	Entity *et;
	Advertisement *at;
	EntityEdit ed;
	AdEdit ad;
	TreeItem *item = tree->getItem(indexof);
	while (wid->WhileSync)
	{

		switch (typealloc)
		{
		case 0://new advertisement
			wid->ihw->close();
			LinkACopy->Name = wid->ihw->line->text().toStdString();
			if (item->data(indexof.column()).toString() == "Broadcasted Ads")
			{

				e->BroadcastedAdvertisement.push_back(std::shared_ptr<Advertisement>(new Advertisement));
				*e->BroadcastedAdvertisement.at(e->BroadcastedAdvertisement.size() - 1) = LinkACopy.get()->createCopy();
				wid->AddNewChild(view, LinkACopy->Name, (void*)e->BroadcastedAdvertisement.at(e->BroadcastedAdvertisement.size() - 1).get(), TreeType::ADVERTISEMENT);
			}
			else if (item->data(indexof.column()).toString() == "Self Ads")
			{

				e->SelfAdvertisment.push_back(std::shared_ptr<Advertisement>(new Advertisement));
				*e->SelfAdvertisment.at(e->SelfAdvertisment.size() - 1) = LinkACopy.get()->createCopy();
				wid->AddNewChild(view, LinkACopy->Name, (void*)e->SelfAdvertisment.at(e->SelfAdvertisment.size() - 1).get(), TreeType::ADVERTISEMENT);
			}
			else if (item->data(indexof.column()).toString() == "Inference")
			{

				this->ad->Infer = new Advertisement();
				*this->ad->Infer = *LinkACopy.get();
				wid->AddNewChild(view, LinkACopy->Name, (void*)&this->ad->Infer, TreeType::ADVERTISEMENT);
			}
			break;
		case 2: //Attribute
			wid->ihw->close();
			LinkEACopy->Name = wid->ihw->line->text().toStdString();
			if (worldptr->AttributeIndex(LinkEACopy->Name) == -1) //This particular attribute hasn't occured before
			{
				worldptr->Attributes->push_back(LinkEACopy->Name);
				worldptr->ResizeEntityAttributes();
				e->UsedAttributes.at(e->UsedAttributes.size() - 1) = worldptr->Attributes->size() - 1;
				e->Attributes.push_back(new EntityAttribute());
				*e->Attributes.at(e->Attributes.size() - 1) = *LinkEACopy.get();
			}
			else
			{
				e->Attributes.push_back(new EntityAttribute());
				*e->Attributes.at(e->Attributes.size() - 1) = *LinkEACopy.get();
				e->UsedAttributes.at(worldptr->AttributeIndex(LinkEACopy->Name)) = worldptr->AttributeIndex(LinkEACopy.get()->Name);

			}
			wid->AddNewChild(view, LinkEACopy->Name, (void*)e->Attributes[e->Attributes.size() - 1], TreeType::ATTRIBUTE);
			break;
		case 3: //Cost
			wid->ihw->close();
			LinkCoCopy->StringT = wid->ihw->line->text().toStdString();
			if (item->data(indexof.column()).toString() == "Positive")
			{
				this->ad->positive_effects.push_back(Cost());
				this->ad->positive_effects.at(this->ad->positive_effects.size() - 1) = LinkCoCopy.get()->createCopy();
				wid->AddNewChild(view, LinkCoCopy->StringT, (void*)&this->ad->positive_effects.at(this->ad->positive_effects.size() - 1), TreeType::COST);
			}
			else if (item->data(indexof.column()).toString() == "Negative")
			{
				this->ad->negative_effects.push_back(Cost());
				this->ad->negative_effects.at(this->ad->negative_effects.size() - 1) = LinkCoCopy.get()->createCopy();
				wid->AddNewChild(view, LinkCoCopy->StringT, (void*)&this->ad->negative_effects.at(this->ad->negative_effects.size() - 1), TreeType::COST);
			}


			break;
		default:

			break;
		}
		wid->WhileSync = false;
	}
}