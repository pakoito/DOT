#ifndef INTEROP_H
#define INTEROP_H

#include <QWidget>
#include <QtWidgets>
#include "../Win32/qcustomplot.h"
#include "../AI_Sample/AILink.h"
#include "GridScene.h"
#include <limits>
#include <float.h>
#define GraphSize 1200
struct GraphData
{
	QVector<double> Y;
};
class Interop : public QWidget
{
    Q_OBJECT
public:
    explicit Interop(int size, QWidget *parent = 0);
	explicit Interop(int size,World *w, QWidget *parent = 0);
    ~Interop();
	QWidget *p;
	World *world;
	GridScene *scene;
private:
	vector<vector<vector<EntityAttribute> > > manygoals;
    vector<QCustomPlot*> widgets;
	vector<vector<GraphData*>> TBG;
    QLineEdit *lineEdit, *lineEdit2;
    vector<QStandardItemModel*> model;
    vector<QTableView*> view;
    runner runs;
    QPushButton *m_button;
	QScrollArea *scroll;
	int w, h;
	void ResetList();
	QCustomPlot *plot;
	int f;
	int EntityCount;
	QVBoxLayout *hLayout3;
	QTabWidget *tabs;
	bool Check;
	bool HasDebugged;
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
	bool PlotFFT(int i, int j, int graphnum = 0);
protected:
    void showEvent(QShowEvent * event);
    void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent * event);
private slots:
void handleButton();
void handleButton2();
void plotPressEvent(QMouseEvent *event, int *val);
void plotReleaseEvent(QMouseEvent *event);

};

#endif // INTEROP_H
