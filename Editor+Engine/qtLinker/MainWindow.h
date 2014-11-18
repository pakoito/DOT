#ifndef MW_H
#define MW_H
#include <QWidget>
#include <QtWidgets>
#include "../AI_Sample/AILink.h"
#include "../Win32/sidebar.h"
#include "runner.h"
#include "TreeView.h"
#include "GridEditor.h"
class MainWindow : public QWidget
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	World* ReturnWorld()
	{
		return world;
	}
	GridEditor* ReturnGrid()
	{
		return grid;
	}
private:
	QLineEdit *lineEdit, *lineEdit2;
	vector<QStandardItemModel*> model;
	vector<QTableView*> view;
	int w, h, f;
	void ResetList();
	QWidget *p;

	QHBoxLayout *vLayout;
	SideBar *mside;

	World *world;
	TreeWindow *tree;
	GridEditor *grid;
protected:

	void paintEvent(QPaintEvent * event);


};
#endif