#ifndef TV_H
#include <QtGui>
#include "TreeViewEntity.h"


TreeWindowEntity::TreeWindowEntity(Entity *e, QObject *parent)
: QAbstractItemModel(parent)
{
	this->e = e;
	QVector<QVariant> rootData;
	rootData << e->Name.c_str();
	rootItem = new TreeItem(rootData);
	setupModelData(rootItem);
}
TreeWindowEntity::TreeWindowEntity(Advertisement *e, QObject *parent)
: QAbstractItemModel(parent)
{
	QVector<QVariant> rootData;
	rootData << e->Name.c_str();
	rootItem = new TreeItem(rootData);
	setupModelData(rootItem,e);
}
TreeWindowEntity::TreeWindowEntity(EntityAttribute *e, QObject *parent)
: QAbstractItemModel(parent)
{
	QVector<QVariant> rootData;
	rootData << "Click 'Gradient' to open the gradient editor";
	rootItem = new TreeItem(rootData);
	setupModelData(rootItem,e);
}
TreeWindowEntity::~TreeWindowEntity()
{
	delete rootItem;
}


int TreeWindowEntity::columnCount(const QModelIndex & /* parent */) const
{
	return rootItem->columnCount();
}

QVariant TreeWindowEntity::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem *item = getItem(index);

	return item->data(index.column());
}

Qt::ItemFlags TreeWindowEntity::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TreeWindowEntity::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QVariant TreeWindowEntity::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeWindowEntity::index(int row, int column, const QModelIndex &parent) const
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

bool TreeWindowEntity::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

bool TreeWindowEntity::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

QModelIndex TreeWindowEntity::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = getItem(index);
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeWindowEntity::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool TreeWindowEntity::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeWindowEntity::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem = getItem(parent);

	return parentItem->childCount();
}

bool TreeWindowEntity::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

bool TreeWindowEntity::setHeaderData(int section, Qt::Orientation orientation,
	const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}
void TreeWindowEntity::setupModelData(TreeItem *parent)
{
	QList<TreeItem*> parents;
	if (parent)
		parents << parent;

	parents.at(0)->insertChildren(0, 2, 1);

	QVector<QVariant> columnData;
	columnData.push_back("Advertisements");
	parents.at(0)->child(0)->setData(0, columnData[0]);


	columnData.clear();
	columnData.push_back("Attributes");
	parents.at(0)->child(1)->type = TreeType::ATTRIBUTEADD;
	parents.at(0)->child(1)->Link = (void*)&e->Attributes;
	parents.at(0)->child(1)->setData(0, columnData[0]);

	columnData.clear();

	//Advertisements
	//parents.at(0)->insertChildren(0, 2, 1);
	QVector<QVariant> addata; //Name
	parents.at(0)->child(0)->insertChildren(0, 2, 1);
	parents.at(0)->child(1)->insertChildren(0, 1, 1);

	columnData.push_back("Broadcasted Ads");
	parents.at(0)->child(0)->child(0)->type = TreeType::ADVERTISEMENTADD;
	parents.at(0)->child(0)->child(0)->Link = (void*)&e->BroadcastedAdvertisement;
	parents.at(0)->child(0)->child(0)->setData(0, columnData[0]);
	columnData.clear();

	columnData.push_back("Self Ads");
	parents.at(0)->child(0)->child(1)->type = TreeType::ADVERTISEMENTADD;
	parents.at(0)->child(0)->child(1)->Link = (void*)&e->SelfAdvertisment;
	parents.at(0)->child(0)->child(1)->setData(0, columnData[0]);
	columnData.clear();

	parents.at(0)->child(0)->child(0)->insertChildren(0, e->BroadcastedAdvertisement.size(), 1); //Create an array of broadcasted ads
	for (int i = 0; i < e->BroadcastedAdvertisement.size(); i++)
	{
		addata.push_back(e->BroadcastedAdvertisement.at(i)->Name.c_str());
		parents.at(0)->child(0)->child(0)->child(i)->setData(0, addata[0]);
		parents.at(0)->child(0)->child(0)->child(i)->type = TreeType::ADVERTISEMENT;
		parents.at(0)->child(0)->child(0)->child(i)->Link = (void*)e->BroadcastedAdvertisement.at(i).get();
		addata.clear();
	}

	parents.at(0)->child(0)->child(1)->insertChildren(0, e->SelfAdvertisment.size(), 1); //Create an array of self ads
	for (int i = 0; i < e->SelfAdvertisment.size(); i++)
	{
		addata.push_back(e->SelfAdvertisment.at(i).get()->Name.c_str());
		parents.at(0)->child(0)->child(1)->child(i)->setData(0, addata[0]);
		parents.at(0)->child(0)->child(1)->child(i)->type = TreeType::ADVERTISEMENT;
		parents.at(0)->child(0)->child(1)->child(i)->Link = (void*)e->SelfAdvertisment.at(i).get();
		addata.clear();
	}

	parents.at(0)->child(1)->insertChildren(0, e->Attributes.size(), 1); //Create an array of attributes
	int k = 0;
	for (int i = 0; i < e->Attributes.size(); i++)
	{
		addata.push_back(e->Attributes.at(i)->Name.c_str());
		parents.at(0)->child(1)->child(k)->setData(0, addata[0]);
		parents.at(0)->child(1)->child(k)->type = TreeType::ATTRIBUTE;
		parents.at(0)->child(1)->child(k)->Link = (void*)e->Attributes.at(i);
		addata.clear();
		k++;
	}
}
void TreeWindowEntity::setupModelData(TreeItem *parent, Advertisement *a)
{
	QList<TreeItem*> parents;
	if (parent)
		parents << parent;

	parents.at(0)->insertChildren(0, 2, 1);

	QVector<QVariant> columnData;
	columnData.push_back("Cost");
	parents.at(0)->child(0)->setData(0, columnData[0]);
	columnData.clear();

	columnData.push_back("Inference");
	parents.at(0)->child(1)->type = TreeType::ADVERTISEMENTADD;
	parents.at(0)->child(1)->Link = (void*)&a->Infer;
	parents.at(0)->child(1)->setData(0, columnData[0]);
	columnData.clear();


	//Advertisements
	//parents.at(0)->insertChildren(0, 2, 1);
	QVector<QVariant> addata; //Name
	parents.at(0)->child(0)->insertChildren(0, 2, 1);

	columnData.push_back("Positive");
	parents.at(0)->child(0)->child(0)->type = TreeType::COSTADD;
	parents.at(0)->child(0)->child(0)->Link = (void*)&a->positive_effects;
	parents.at(0)->child(0)->child(0)->setData(0, columnData[0]);
	columnData.clear();

	columnData.push_back("Negative");
	parents.at(0)->child(0)->child(1)->type = TreeType::COSTADD;
	parents.at(0)->child(0)->child(1)->Link = (void*)&a->negative_effects;
	parents.at(0)->child(0)->child(1)->setData(0, columnData[0]);
	columnData.clear();


	parents.at(0)->child(0)->child(0)->insertChildren(0, a->positive_effects.size(), 1); //Create an array of pos 
	for (int i = 0; i < a->positive_effects.size(); i++)
	{
		addata.push_back(a->positive_effects.at(i).StringT.c_str());
		parents.at(0)->child(0)->child(0)->child(i)->setData(0, addata[0]);
		parents.at(0)->child(0)->child(0)->child(i)->type = TreeType::COST;
		parents.at(0)->child(0)->child(0)->child(i)->Link = (void*)&a->positive_effects.at(i);
		addata.clear();
	}

	parents.at(0)->child(0)->child(1)->insertChildren(0, a->negative_effects.size(), 1); //Create an array of neg 
	for (int i = 0; i < a->negative_effects.size(); i++)
	{
		addata.push_back(a->negative_effects.at(i).StringT.c_str());
		parents.at(0)->child(0)->child(1)->child(i)->setData(0, addata[0]);
		parents.at(0)->child(0)->child(1)->child(i)->type = TreeType::COST;
		parents.at(0)->child(0)->child(1)->child(i)->Link = (void*)&a->negative_effects.at(i);
		addata.clear();
	}
	
	if (a->Infer != nullptr)
	{
		parents.at(0)->child(1)->insertChildren(0, 1, 1); //Create an array of neg 
		addata.push_back(a->Infer->Name.c_str());
		parents.at(0)->child(1)->child(0)->setData(0, addata[0]);
		parents.at(0)->child(1)->child(0)->type = TreeType::ADVERTISEMENT;
		parents.at(0)->child(1)->child(0)->Link = (void*)a->Infer;
		addata.clear();
	}
}
void TreeWindowEntity::setupModelData(TreeItem *parent, EntityAttribute *a)
{
	QList<TreeItem*> parents;
	if (parent)
		parents << parent;

	parents.at(0)->insertChildren(0, 1, 1);

	QVector<QVariant> columnData;
	columnData.push_back("Gradient");
	parents.at(0)->child(0)->setData(0, columnData[0]);
	parents.at(0)->child(0)->Link = (void*)&a->grad;
	parents.at(0)->child(0)->type = TreeType::GRAD;
	columnData.clear();

	//Advertisements
	//parents.at(0)->insertChildren(0, 2, 1);
	QVector<QVariant> addata; //Name
	parents.at(0)->child(0)->insertChildren(0, 6, 1);

	columnData.push_back("Low: " + (QString)std::to_string(a->grad.Low).c_str());
	parents.at(0)->child(0)->child(0)->setData(0, columnData[0]);
	columnData.clear();

	columnData.push_back("First Quartile: " + (QString)std::to_string(a->grad.Q1).c_str());
	parents.at(0)->child(0)->child(1)->setData(0, columnData[0]);
	columnData.clear();

	columnData.push_back("Third Quartile: " + (QString)std::to_string(a->grad.Q3).c_str());
	parents.at(0)->child(0)->child(2)->setData(0, columnData[0]);
	columnData.clear();

	columnData.push_back("Max: " + (QString)std::to_string(a->grad.Max).c_str());
	parents.at(0)->child(0)->child(3)->setData(0, columnData[0]);
	columnData.clear();
}
#endif