#ifndef TW_H
#define TW_H
#include <QWidget>
#include <QtWidgets>
#include "../AI_Sample/AILink.h"
#include "../Win32/sidebar.h"
#include "runner.h"
#include "TreeView.h"
#include "Inheritance.h"
#include "ObjectEditor.h"
#include "GridScene.h"
class TreeWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TreeWidget(World *wo, QWidget *parent = 0);
	~TreeWidget();

	int w, h, f;
	QWidget *p;

	std::shared_ptr<Entity> LinkECopy;
	std::shared_ptr<Advertisement> LinkACopy;
	std::shared_ptr<EntityAttribute> LinkEACopy;
	std::shared_ptr<Cost> LinkCoCopy;
	bool WasHiden;
	void AddNewChild(std::string Name, void *Link, int type)
	{
		QModelIndex index = view->selectionModel()->currentIndex();
		QAbstractItemModel *model = view->model();

		if (model->columnCount(index) == 0) {
			if (!model->insertColumn(0, index))
				return;
		}

		if (!model->insertRow(0, index))
			return;

		for (int column = 0; column < model->columnCount(index); ++column) {
			QModelIndex child = model->index(0, column, index);
			model->setData(child, QVariant(QString::fromStdString(Name)), Qt::EditRole);

			if (!model->headerData(column, Qt::Horizontal).isValid())
				model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
				Qt::EditRole);
		}

		view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
			QItemSelectionModel::ClearAndSelect);
		tree->getItem(model->index(0, 0, index))->Link = Link;
		tree->getItem(model->index(0, 0, index))->type = type;
	}
	void AddNewChild(QTreeView *treeview, std::string Name, void *Link, int type)
	{
		QModelIndex index = treeview->selectionModel()->currentIndex();
		QAbstractItemModel *model = treeview->model();

		if (model->columnCount(index) == 0) {
			if (!model->insertColumn(0, index))
				return;
		}

		if (!model->insertRow(0, index))
			return;

		for (int column = 0; column < model->columnCount(index); ++column) {
			QModelIndex child = model->index(0, column, index);
			model->setData(child, QVariant(QString::fromStdString(Name)), Qt::EditRole);

			if (!model->headerData(column, Qt::Horizontal).isValid())
				model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
				Qt::EditRole);
		}

		treeview->selectionModel()->setCurrentIndex(model->index(0, 0, index),
			QItemSelectionModel::ClearAndSelect);
		tree->getItem(model->index(0, 0, index))->Link = Link;
		tree->getItem(model->index(0, 0, index))->type = type;
	}

	void DeleteAd(std::string Name)
	{
		bool check = false;
		for (int i = 0; i < worldptr->SimulationAds->size(); i++)
		{
			if (Name == worldptr->SimulationAds->at(i).get()->Name)
			{
				check = true;
				worldptr->SimulationAds->erase(worldptr->SimulationAds->begin() + i);
				break;
			}
		}
		if (check == true)
			DeleteAd(Name);
	}
	void DeleteEntity(std::string Name)
	{
		bool check = false;
		for (int i = 0; i < worldptr->SimulationEntities->size(); i++)
		{
			if (Name == worldptr->SimulationEntities->at(i).get()->Name)
			{
				check = true;
				worldptr->SimulationEntities->erase(worldptr->SimulationEntities->begin() + i);
				break;
			}
		}
		if (check == true)
			DeleteEntity(Name);
	}
	int typealloc;
	bool WhileSync;
	bool Copied;
	InheritanceWidget *ihw;
	int AdIndex;
	GridScene *scene;
private:
	TreeWindow *tree;
	QTreeView *view;
	QHBoxLayout *vLayout;
	World *worldptr;
protected:
	Entity *LinkE;
	Advertisement *LinkA;
	EntityAttribute *LinkEA;
	void keyPressEvent(QKeyEvent *event)
	{
		evt.push_back(event->key());
	}
	void keyReleaseEvent(QKeyEvent *keyevent)
	{
		for (int i = 0; i < evt.size(); i++)
		{
			if (evt.at(i) == keyevent->key())
			{
				evt.erase(evt.begin() + i);
			}
		}
	}
	QVector<int> evt;


	int currentlyused;

	void paintEvent(QPaintEvent * event);
	ObjectEditor *ob;
	QModelIndex indexat;

private slots:
	void OnClick(const QModelIndex&);
	void hideEvent(QHideEvent *event);
	void CreateNew()
	{
		QPointF point;
		Entity *et;
		Advertisement *at;
		EntityEdit ed;
		AdEdit ad;
		while (WhileSync)
		{

			switch (typealloc)
			{
			case 0://new advertisement
				ihw->close();
				LinkACopy->Name = ihw->line->text().toStdString();
				worldptr->AddAdvertisement();
				*worldptr->Ads->at(worldptr->Ads->size() - 1) = LinkACopy.get()->createCopy();
				AddNewChild(LinkACopy->Name, (void*)worldptr->Ads->at(worldptr->Ads->size() - 1).get(), TreeType::ADVERTISEMENT);
				break;
			case 1: //Entity
				ihw->close();
				LinkECopy->Name = ihw->line->text().toStdString();
				worldptr->AddEntity();
				*worldptr->Entities->at(worldptr->Entities->size() - 1) = LinkECopy.get()->createCopy();
				AddNewChild(LinkECopy->Name, (void*)worldptr->Entities->at(worldptr->Entities->size() - 1).get(), TreeType::ENTITY);
				break;
			default:

				break;
			}
			WhileSync = false;
		}
	}
};
#endif