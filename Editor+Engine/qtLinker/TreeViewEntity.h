#ifndef TVE_H
#define TVE_H
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "../AI_Sample/World.h"
#include "TreeItem.h"

class TreeWindowEntity : public QAbstractItemModel
{
	Q_OBJECT
public:
	TreeWindowEntity(Entity *e, QObject *parent = 0);
	TreeWindowEntity(EntityAttribute *e, QObject *parent = 0);
	TreeWindowEntity(Cost *e, QObject *parent = 0);
	TreeWindowEntity(Advertisement *e, QObject *parent = 0);
	~TreeWindowEntity();

	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value,
		int role = Qt::EditRole);
	bool setHeaderData(int section, Qt::Orientation orientation,
		const QVariant &value, int role = Qt::EditRole);

	bool insertColumns(int position, int columns,
		const QModelIndex &parent = QModelIndex());
	bool removeColumns(int position, int columns,
		const QModelIndex &parent = QModelIndex());
	bool insertRows(int position, int rows,
		const QModelIndex &parent = QModelIndex());
	bool removeRows(int position, int rows,
		const QModelIndex &parent = QModelIndex());
	Entity *e;
	TreeItem *getItem(const QModelIndex &index) const;
private:
	void setupModelData(TreeItem *parent);
	void setupModelData(TreeItem *parent, Cost *ea);
	void setupModelData(TreeItem *parent, Advertisement *ea);
	void setupModelData(TreeItem *parent, EntityAttribute *ea);
	TreeItem *rootItem;
};
#endif