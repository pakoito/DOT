#include "MainWindow.h"
#include <iostream>
#include <QCoreApplication>
#include <qtconcurrentrun.h>
#include <QThread>
#include "TreeWidget.h"
MainWindow::MainWindow(QWidget *parent) :
QWidget(parent)
{
	resize(1280, 960);
	w = width();
	h = height();
	p = parent;
	f = 0;

	vLayout = new QHBoxLayout(this);

	world = new World();
	runner r = runner();
	r.startup(world);

	mside = new SideBar(vLayout);
	mside->world = world;
	mside->p = this;

	TreeWidget *tree = new TreeWidget(world);
	tree->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
	tree->p = this;
	tree->WasHiden = true;
	grid = new GridEditor(world);
	grid->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
	grid->p = this;

	Interop *q = new Interop(world->SimulationEntities->size());
	q->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
	q->p = this;
	q->world = world; //Set Interop's pointer
	q->scene = grid->grid;

	SideBar *msideFile = new SideBar(vLayout);
	msideFile->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));

	SideBar *msideFileFromXML = new SideBar(vLayout);
	msideFileFromXML->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
	
	SideBar *mFileToXML = new SideBar(vLayout);
	mFileToXML->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));

	tree->scene = grid->grid;

	//mside->addAction("Build Project");
	//mside->addAction("L.O.D.");
	mside->setFixedWidth(75);
	
	//Tier 1
	mside->gs = grid->grid;
	mside->addAction("File", msideFile);
	mside->addAction("Debug", q, (Qt::AlignmentFlag)0);
	mside->addAction("Visual Editor", grid, Qt::AlignTop);
	mside->addAction("Tree View", tree, Qt::AlignTop);

	//Tier 2 (File->)
	msideFile->addAction("Load", msideFileFromXML, Qt::AlignLeft);
	msideFile->addAction("Save",mFileToXML, Qt::AlignLeft);

	//Tier 3 (File->Load)
	msideFileFromXML->world = world;
	msideFileFromXML->gs = grid->grid;
	msideFileFromXML->addAction("From XML", msideFileFromXML, Qt::AlignLeft);
	
	//Tier 3  (File->Save)
	mFileToXML->world = world;
	mFileToXML->addAction("To XML", mFileToXML, Qt::AlignLeft);

	vLayout->addWidget(mside, 0, Qt::AlignLeft);
	vLayout->setAlignment(Qt::AlignLeft);
	mside->RefLayout = vLayout;

}
MainWindow::~MainWindow()
{
	_sleep(10);

}
void MainWindow::paintEvent(QPaintEvent * event)
{

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
				this->setFixedSize(p->width()/* * w2*/ - 50, p->height() - 25 /** h2*/);
			}
		}
		w = p->width();
		h = p->height();
		f = 0;
	};
	f++;
}