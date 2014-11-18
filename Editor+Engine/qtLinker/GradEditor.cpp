#include <iostream>
#include <QCoreApplication>
#include <qtconcurrentrun.h>
#include <QThread>
#include "GradEditor.h"
#include "SliderBoxWidget.h"
GradientEditor2DAdvanced::GradientEditor2DAdvanced(World *wo, Gradient *grad, QWidget *parent)
{
	this->link = wo;
	this->g = grad;
	resize(1280, 960);
	this->p = parent;
	this->vLayout = new QVBoxLayout(this);
	GraphOrConfirm = false;
	
#ifndef ADV_2
	graph = new QCustomPlot();
	graph->xAxis->setRange(0, 1);
	graph->yAxis->setRange(-1, 2);
	graph->setMinimumWidth(200);
	graph->setMinimumHeight(200);
	graph->setMaximumHeight(400);
	graph->addGraph();
	vLayout->addWidget(graph, 0, Qt::AlignTop);
	newTB("Low", (void*)&std::to_string(grad->Low), Link::INT);
	newTB("First Quartile", (void*)&std::to_string(grad->Q1), Link::INT);
	newTB("Third Quartile", (void*)&std::to_string(grad->Q3), Link::INT);
	newTB("Max", (void*)&std::to_string(grad->Max), Link::INT);
	sbw = new SliderBoxWidget("Z: .2", 20);
	vLayout->addWidget(sbw);
	newTB("Current value", (void*)&std::to_string(200), Link::INT);
	m_button = new QPushButton("Update");
	vLayout->addWidget(m_button, 0, Qt::AlignBottom);
	connect(m_button, SIGNAL(clicked()), this, SLOT(GraphButtonPress()));
	for (int i = 0; i < textboxes.size(); i++)
	{
		connect(textboxes[i].txt->Title2, SIGNAL(editingFinished()), SLOT(TextChanged()));
		connect(textboxes[i].txt->Title2, SIGNAL(returnPressed()), SLOT(TextChanged()));
	}
	connect(sbw->Title2, SIGNAL(sliderReleased()), SLOT(TextChanged()));
	TextChanged();
	update();
#endif
};
