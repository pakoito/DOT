#ifndef TV_H
#include <QtGui>
#include "TreeView.h"


TreeWindow::TreeWindow(World *world, QObject *parent)
: QAbstractItemModel(parent)
{
	wo = world;
	QVector<QVariant> rootData;
	rootData << "World";
	rootItem = new TreeItem(rootData);
	setupModelData(rootItem);
}

TreeWindow::~TreeWindow()
{
	delete rootItem;
}


int TreeWindow::columnCount(const QModelIndex & /* parent */) const
{
	return rootItem->columnCount();
}

QVariant TreeWindow::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem *item = getItem(index);

	return item->data(index.column());
}

Qt::ItemFlags TreeWindow::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TreeWindow::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QVariant TreeWindow::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeWindow::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	TreeItem *parentItem = getItem(parent);

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

bool TreeWindow::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

bool TreeWindow::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

QModelIndex TreeWindow::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = getItem(index);
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeWindow::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool TreeWindow::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeWindow::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem = getItem(parent);

	return parentItem->childCount();
}

bool TreeWindow::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

bool TreeWindow::setHeaderData(int section, Qt::Orientation orientation,
	const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}
void TreeWindow::setupModelData(TreeItem *parent)
{
	QList<TreeItem*> parents;
	if (parent)
		parents << parent;

	parents.at(0)->insertChildren(0, 2, 1);

	QVector<QVariant> columnData;
	columnData.push_back("Advertisements");
	parents.at(0)->child(0)->setData(0, columnData[0]);
	parents.at(0)->child(0)->type = TreeType::ADVERTISEMENTADD;

	columnData.clear();
	columnData.push_back("Agent");
	parents.at(0)->child(1)->setData(0, columnData[0]);
	parents.at(0)->child(1)->type = TreeType::ENTITYADD;

	columnData.clear();

	//Advertisements
	//parents.at(0)->insertChildren(0, 2, 1);
	QVector<QVariant> addata; //Name
	parents.at(0)->child(0)->insertChildren(0, wo->Ads->size(), 1);
	for (int i = 0; i < wo->Ads->size(); i++)
	{
		addata.push_back(wo->Ads->at(i).get()->Name.c_str());
		parents.at(0)->child(0)->child(i)->setData(0, addata[0]);
		parents.at(0)->child(0)->child(i)->type = TreeType::ADVERTISEMENT;
		parents.at(0)->child(0)->child(i)->Link = (void*)wo->Ads->at(i).get();
		addata.clear();
	}
	
	//Entities
	QList<QVariant> addata2; //Name
	parents.at(0)->child(1)->insertChildren(0, wo->Entities->size(), 1);
	for (int i = 0; i < wo->Entities->size(); i++)
	{
		addata2.push_back(wo->Entities->at(i).get()->Name.c_str());
		parents.at(0)->child(1)->child(i)->setData(0, addata2[0]);
		parents.at(0)->child(1)->child(i)->type = TreeType::ENTITY;
		parents.at(0)->child(1)->child(i)->Link = (void*)wo->Entities->at(i).get();
		addata2.clear();
	}

}
#endif