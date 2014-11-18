#ifndef TV_H
#define TV_H
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "../AI_Sample/World.h"
#include "TreeItem.h"

class TreeWindow : public QAbstractItemModel
{
	Q_OBJECT
public:
	TreeWindow(World *world, QObject *parent = 0);
	~TreeWindow();

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
	World *wo;
	TreeItem *getItem(const QModelIndex &index) const;
private:
	void setupModelData(TreeItem *parent);



	TreeItem *rootItem;
};
#endif