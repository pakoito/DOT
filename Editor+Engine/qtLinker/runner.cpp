#include "runner.h"
#include <iostream>
#include <QCoreApplication>
#include "WriteXML.h"
#include "Itter.h"


Interop::Interop(int size, QWidget *parent) :
QWidget(parent)
{
	Check = false;
	HasDebugged = false;
	resize(1280, 960);
	w = width();
	h = height();
	scroll = new QScrollArea();
	scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	p = parent;
	hLayout3 = new QVBoxLayout;

	QVBoxLayout *controlLayout = new QVBoxLayout;
	QVBoxLayout *controlLayoutSec = new QVBoxLayout;

	hLayout3->addLayout(controlLayout);
	hLayout3->addLayout(controlLayoutSec);

	QWidget *baseArea = new QWidget();
	for (int i = 0; i < size; ++i)
	{
		TBG.push_back(vector<GraphData*>());
		widgets.push_back(new QCustomPlot());
		manygoals.push_back(vector<vector<EntityAttribute> >());
		int j = 0;
		for (int j = 0; j < world->SimulationEntities->at(i).get()->Attributes.size(); j++)
		{
			float col = 16 * ((float)j / (float)world->SimulationEntities->at(i)->Attributes.size());
			col += 2;
			widgets[i]->addGraph();
			if (Qt::GlobalColor((int)col) == Qt::yellow || Qt::GlobalColor((int)col) == Qt::GlobalColor::white || Qt::GlobalColor((int)col) == Qt::GlobalColor::lightGray)
				col += 1;
			widgets[i]->graph(j)->setName(QString::fromStdString(world->Attributes->at(j).Name));
			widgets[i]->graph(j)->setPen(QPen(Qt::GlobalColor((int)col)));
			TBG[i].push_back(new GraphData());
			widgets[i]->setInteraction(QCP::Interaction::iRangeZoom, true);
			widgets[i]->setInteraction(QCP::Interaction::iRangeDrag, true);
			widgets[i]->val1 = i;
		}

		widgets[i]->xAxis->setLabel("Time");
		widgets[i]->yAxis->setLabel("Value");

		widgets[i]->xAxis->setRange(0, 1200);
		widgets[i]->yAxis->setRange(0, 300);
		widgets[i]->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
		widgets[i]->setMinimumSize(1220, 300);
		connect(widgets[i], SIGNAL(mousePress(QMouseEvent*, int)), this, SLOT(plotPressEvent(QMouseEvent*, int)));
		connect(widgets[i], SIGNAL(mouseRelease(QMouseEvent*, int)), this, SLOT(plotReleaseEvent(QMouseEvent*, int)));
		//widgets[i]->setLayout(hLayout3);

		hLayout3->addWidget(widgets[i]);
		hLayout3->setAlignment(widgets[i], Qt::AlignmentFlag::AlignTop);


	}
	hLayout3->setAlignment(Qt::AlignmentFlag::AlignTop);
	baseArea->setLayout(hLayout3);


	scroll->setWidget(baseArea);
	scroll->widget()->setLayout(hLayout3);
	scroll->setWidgetResizable(true);
	scroll->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
	QHBoxLayout *hLayoutScrollArea = new QHBoxLayout;

	hLayoutScrollArea->addWidget(scroll);
	QLabel *secondLbl = new QLabel();
	secondLbl->setText(" ");
	this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
	QVBoxLayout *vLayout = new QVBoxLayout(this);

	vLayout->addLayout(hLayoutScrollArea);
	vLayout->addWidget(secondLbl);

	lineEdit2 = new QLineEdit("How many itterations do you want? (Enter a number before pressing simulate)");
	vLayout->addWidget(lineEdit2, 0, Qt::AlignBottom);
	m_button = new QPushButton("Run with Global Updates");
	vLayout->addWidget(m_button, 0, Qt::AlignBottom);
	QPushButton *m_button2 = new QPushButton("Run without Global Updates");
	vLayout->addWidget(m_button2, 0, Qt::AlignBottom);


	connect(m_button, SIGNAL(clicked()), this, SLOT(handleButton()));
	connect(m_button2, SIGNAL(clicked()), this, SLOT(handleButton2()));
	f = 0;
	plot = new QCustomPlot;

	plot->addGraph();
	plot->addGraph();
	plot->addGraph();
	plot->addGraph();
	plot->addGraph();
	plot->addGraph();

	plot->setInteraction(QCP::Interaction::iRangeZoom, true);
	plot->setInteraction(QCP::Interaction::iRangeDrag, true);

	plot->xAxis->setLabel("Time");
	plot->yAxis->setLabel("Value");

	plot->xAxis->setRange(0, 7);
	plot->yAxis->setRange(-200, 200);
	plot->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
	plot->setMinimumSize(1220, 300);
	plot->setWindowTitle("Fourier Transform");
}
Interop::Interop(int size, World *wo, QWidget *parent) :
QWidget(parent)
{
	Check = false;
	HasDebugged = false;
	resize(1280, 960);
	w = width();
	h = height();
	this->world = wo;
	scroll = new QScrollArea();
	scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	p = parent;
	hLayout3 = new QVBoxLayout;

	QVBoxLayout *controlLayout = new QVBoxLayout;
	QVBoxLayout *controlLayoutSec = new QVBoxLayout;

	hLayout3->addLayout(controlLayout);
	hLayout3->addLayout(controlLayoutSec);

	QWidget *baseArea = new QWidget();
	for (int i = 0; i < size; ++i)
	{
		TBG.push_back(vector<GraphData*>());
		widgets.push_back(new QCustomPlot());
		manygoals.push_back(vector<vector<EntityAttribute> >());
		int j = 0;
		for (int j = 0; j < world->SimulationEntities->at(i).get()->Attributes.size(); j++)
		{
			float col = 16 * ((float)j / (float)world->SimulationEntities->at(i)->Attributes.size());
			col += 2;
			widgets[i]->addGraph();
			if (Qt::GlobalColor((int)col) == Qt::yellow || Qt::GlobalColor((int)col) == Qt::GlobalColor::white || Qt::GlobalColor((int)col) == Qt::GlobalColor::lightGray)
				col += 1;
			widgets[i]->graph(j)->setName(QString::fromStdString(world->Attributes->at(j).Name));
			widgets[i]->graph(j)->setPen(QPen(Qt::GlobalColor((int)col)));
			TBG[i].push_back(new GraphData());
			widgets[i]->setInteraction(QCP::Interaction::iRangeZoom, true);
			widgets[i]->setInteraction(QCP::Interaction::iRangeDrag, true);
			widgets[i]->setInteraction(QCP::Interaction::iSelectAxes, true);
			widgets[i]->val1 = i;
		}

		widgets[i]->xAxis->setLabel("Time");
		widgets[i]->yAxis->setLabel("Value");

		widgets[i]->xAxis->setRange(0, 1200);
		widgets[i]->yAxis->setRange(0, 300);
		widgets[i]->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
		widgets[i]->setMinimumSize(1220, 300);
		connect(widgets[i], SIGNAL(mousePress(QMouseEvent*, int*)), this, SLOT(plotPressEvent(QMouseEvent*, int*)));
		connect(widgets[i], SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(plotReleaseEvent(QMouseEvent*)));
		//widgets[i]->setLayout(hLayout3);

		hLayout3->addWidget(widgets[i]);
		hLayout3->setAlignment(widgets[i], Qt::AlignmentFlag::AlignTop);
	}
	hLayout3->setAlignment(Qt::AlignmentFlag::AlignTop);
	baseArea->setLayout(hLayout3);


	scroll->setWidget(baseArea);
	scroll->widget()->setLayout(hLayout3);
	scroll->setWidgetResizable(true);
	scroll->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
	QHBoxLayout *hLayoutScrollArea = new QHBoxLayout;

	hLayoutScrollArea->addWidget(scroll);
	QLabel *secondLbl = new QLabel();
	secondLbl->setText(" ");
	this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding));
	QVBoxLayout *vLayout = new QVBoxLayout(this);

	vLayout->addLayout(hLayoutScrollArea);
	vLayout->addWidget(secondLbl);

	lineEdit2 = new QLineEdit("How many itterations do you want? (Enter a number before pressing simulate)");
	vLayout->addWidget(lineEdit2, 0, Qt::AlignBottom);
	m_button = new QPushButton("Run with Global Updates");
	vLayout->addWidget(m_button, 0, Qt::AlignBottom);
	QPushButton *m_button2 = new QPushButton("Run without Global Updates");
	vLayout->addWidget(m_button2, 0, Qt::AlignBottom);


	connect(m_button, SIGNAL(clicked()), this, SLOT(handleButton()));
	connect(m_button2, SIGNAL(clicked()), this, SLOT(handleButton2()));
	f = 0;
	plot = new QCustomPlot();

	plot->addGraph();
	plot->addGraph();
	plot->addGraph();
	plot->addGraph();
	plot->addGraph();
	plot->addGraph();

	plot->setInteraction(QCP::Interaction::iRangeZoom, true);
	plot->setInteraction(QCP::Interaction::iRangeDrag, true);
	plot->setInteraction(QCP::Interaction::iSelectAxes, true);

	plot->xAxis->setLabel("Time");
	plot->yAxis->setLabel("Value");

	plot->xAxis->setRange(0, 12);
	plot->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding));
	plot->setMinimumSize(1220, 300);
	tabs = new QTabWidget();
}
Interop::~Interop()
{
	_sleep(10);

}
void ClearGraph(QLayout *layout)
{
	QLayoutItem *item;
	while ((item = layout->takeAt(0))) {
		if (item->layout()) {
			ClearGraph(item->layout());
			delete item->layout();
		}
		else if (item->widget()) {
			delete item->widget();
		}
		try
		{
			delete item;
		}
		catch (exception e)
		{
		};
	}
}
void Interop::paintEvent(QPaintEvent * event)
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
				//this->setFixedSize((p->width() / w2) - 100, p->height() / h2);
			}
		}
		w = p->width();
		h = p->height();
		f = 0;
	};
	f++;
}

void Interop::showEvent(QShowEvent * event)
{
}
void Interop::resizeEvent(QResizeEvent * event)
{
}
bool threadRun(QLineEdit* lineEdit, vector<vector<GraphData*>> *TBG, int max, World currentworld)
{
	std::clock_t start;
	double duration;

	start = std::clock();
	std::clock_t start2 = std::clock();

	bool outputted = false;
	int m = 0;
	currentworld.ScoreNum = 0;
	while (m <= max)
	{
		currentworld.Update_No_LOD(600);

		for (int column = 0; column < currentworld.SimulationEntities->size(); ++column) {
			//QList<QStandardItem*> ritem;z
			if (!currentworld.SimulationEntities->at(column).get()->IsDead())
			{
				for (int i = 0; i < currentworld.SimulationEntities->at(column)->Attributes.size(); i++)
				{
					TBG->at(column).at(i)->Y.push_back(currentworld.SimulationEntities->at(column)->Attributes.at(i)->Value);
				}
			}
		}
		m++;
	}
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	double duration2 = (std::clock() - start2) / (double)CLOCKS_PER_SEC;
	int q = (int)(currentworld.ScoreNum / duration);
	
	qDebug() << "" << q << " scores calculated, time: " << (float)duration2 << " seconds\n";

	return false;

}
void Interop::ResetList()
{
	TBG = vector<vector<GraphData*>>();
	for (int i = 0; i < world->SimulationEntities->size(); i++)
	{
		TBG.push_back(vector<GraphData*>());
		for (int j = 0; j < world->SimulationEntities->at(i).get()->Attributes.size(); j++)
		{
			TBG[i].push_back(new GraphData());
			TBG[i][j]->Y = QVector<double>();
		}
	}
};
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y_%m_%d", &tstruct);

	return buf;
}
void Interop::handleButton()
{
	ResetList();
	int t = lineEdit2->text().toInt();

	QString DocumentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	QDir dir(DocumentsFolder + QString::fromStdString("/DOT/Backups/"));
	dir.mkpath(DocumentsFolder + QString::fromStdString("/DOT/Backups/"));
	XMLHandler handle = XMLHandler(DocumentsFolder + QString::fromStdString("/DOT/Backups/Backup_" + currentDateTime() + std::string(".xml")), world); //Create a backup
	handle = XMLHandler(DocumentsFolder + "/DOT/temp_update.xml", world);
	handle = XMLHandler(DocumentsFolder + "/DOT/temp_update.xml", world, scene, false);
	world->Build();
	if (!threadRun(lineEdit2, &TBG, t, *world))
	{

		QVector <double> f(t);
		for (int i = 0; i < t; i++)
		{
			f[i] = i;
		}
		for (int i = 0; i < TBG.size(); i++) //Every entity
		{
			for (int k = 0; k < TBG.at(i).size(); k++) //Every attribute
			{
				widgets[i]->graph(k)->setData(f, TBG[i][k]->Y);
			}
			widgets[i]->xAxis->setRange(0, t);
			widgets[i]->replot();
		}
		HasDebugged = true;
	}
	else
	{
		QMessageBox(QMessageBox::NoIcon, "Error!", "Be sure to add entities to the simulation list from the visual editor!", QMessageBox::StandardButton::Ok, this).exec();
	}
	handle.ptr = scene;
	handle = XMLHandler(DocumentsFolder + "/DOT/temp_update.xml", world);
}
void Interop::handleButton2()
{

	ResetList();
	int t = lineEdit2->text().toInt();

	QString DocumentsFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	QDir dir(DocumentsFolder + QString::fromStdString("/DOT/Backups/"));
	dir.mkpath(DocumentsFolder + QString::fromStdString("/DOT/Backups/"));
	XMLHandler handle = XMLHandler(DocumentsFolder + QString::fromStdString("/DOT/Backups/Backup_" + currentDateTime() + std::string(".xml")), world); //Create a backup
	handle = XMLHandler(DocumentsFolder + "/DOT/temp_noupdate.xml", world);
	handle = XMLHandler(DocumentsFolder + "/DOT/temp_noupdate.xml", world, scene, false);
	world->Build();
	if (!threadRun(lineEdit2, &TBG, t, *world))
	{

		QVector <double> f(t);
		for (int i = 0; i < t; i++)
		{
			f[i] = i;
		}
		for (int i = 0; i < TBG.size(); i++) //Every entity
		{
			for (int k = 0; k < TBG.at(i).size(); k++) //Every attribute
			{
				widgets[i]->graph(k)->setData(f, TBG[i][k]->Y);
			}
			widgets[i]->xAxis->setRange(0, t);
			widgets[i]->replot();
		}
		HasDebugged = true;
	}
	else
		QMessageBox(QMessageBox::NoIcon, "Error!", "Be sure to add entities to the simulation list from the visual editor!", QMessageBox::StandardButton::Ok, this).exec();
	handle.ptr = scene;
	handle = XMLHandler(DocumentsFolder + "/DOT/temp_noupdate.xml", world, scene, false);
}
bool Interop::PlotFFT(int i, int j, int graphnum)
{

	typedef complex<double> cx;
	QVector<cx> vec;
	for (int k = 0; k < TBG[i][j]->Y.size(); k++)
	{
		if (k != 0)
		{
			double slope = TBG[i][j]->Y.at(k) - TBG[i][j]->Y.at(k - 1);
			if (slope != 0) //No horizontal tangent line
			{
				vec.push_back((cx(k, TBG[i][j]->Y.at(k))));
			}
		}

	}
	if (vec.size() < 10)
		return false;
	if (vec.size() != 0)
	{

		cx *out = new cx[vec.size()];
		fft(&vec[0], out, 4);
		QVector<double> re;
		QVector<double> im;
		double max = 0;
		double min = 0;
		for (int k = 0; k < vec.size(); k++)
		{
		restartc:
			double real = out[k].real();
			double imag = out[k].imag();
			int error = 2;

			//Check for errors
			if (real > 10000){
				real = 10000;
				error -= 1;
			}
			if (imag > 10000){
				imag = 10000;
				error -= 1;
			}
			if (real < -10000){
				real = -10000;
				error -= 1;
			}
			if (imag < -10000){
				imag = -10000;
				error -= 1;
			}
			if (error <= 0){
				k++;
				if (k < vec.size())
					goto deletearr;
				else
					break;
			}
			re.push_back(real);
			im.push_back(imag);

			if (re.at(k) > max)
				max = re.at(k);
			if (im.at(k) > max)
				max = im.at(k);

			if (re.at(k) < min)
				min = re.at(k);
			if (im.at(k) < min)
				min = im.at(k);
		}
		if (re.size() != 0)
		{
			QVector <double> f(re.size());
			for (int k = 0; k < re.size(); k++)
			{
				f[k] = k;
			}
			if (plot->yAxis->range().upper < max + 50)
				plot->yAxis->setRangeUpper(max + 50);
			if (plot->yAxis->range().lower > min - 50)
				plot->yAxis->setRangeLower(min - 50);

			plot->graph(graphnum)->setPen(QPen(Qt::GlobalColor::blue));
			plot->graph(graphnum)->setData(f, re);
			plot->graph(graphnum + 1)->setPen(QPen(Qt::GlobalColor::red));
			plot->graph(graphnum + 1)->setData(f, im);
			return true;
		}
deletearr:
		out = nullptr;

		re.~QVector();
		im.~QVector();

		delete out;
		return false;
	}
};

void Interop::plotPressEvent(QMouseEvent *event, int *widget)
{
	while (Check)
	{
		if (event->button() == Qt::RightButton)
		{
			QCustomPlot *temp = nullptr;
			QCPAbstractPlottable *plottable;

			plottable =
				widgets[*widget]->plottableAt(event->pos());
			if (plottable)
				temp = widgets[*widget];

			if (temp == nullptr)
				break;
			double x = temp->xAxis->pixelToCoord(event->pos().x());
			double y = temp->yAxis->pixelToCoord(event->pos().y());
			QCPGraph *graph =
				qobject_cast<QCPGraph*>(plottable);
			int j;
			for (j = 0; j < widgets[*widget]->graphCount(); j++)
			{
				if (graph->name() == widgets[*widget]->graph(j)->name())
					break;
			}
			bool ControlDown = false;
			bool ADown = false;
			for (int k = 0; k < evt.size(); k++)
			{
				if (evt.at(k) == Qt::Key::Key_Control)
					ControlDown = true;
				if (evt.at(k) == Qt::Key::Key_A)
					ADown = true;
			}

			if (ControlDown) //Perform an FFT
			{
				if (!ADown)
				{
					QCustomPlot *plot1 = new QCustomPlot();

					plot->addGraph();
					plot->addGraph();
					PlotFFT(*widget, j, plot->graphCount()- 2);
					plot1->addGraph();
					plot1->graph(plot1->graphCount() - 1)->setData(widgets.at(*widget)->graph(j)->data(), true);
					plot1->graph(plot1->graphCount() - 1)->setPen(widgets[*widget]->graph(j)->pen());
					plot1->graph(plot1->graphCount() - 1)->setName(widgets[*widget]->graph(j)->name());

					plot->replot();


					plot1->replot();
					plot1->setInteraction(QCP::Interaction::iRangeZoom, true);
					plot1->setInteraction(QCP::Interaction::iRangeDrag, true);
					plot1->setInteraction(QCP::Interaction::iSelectAxes, true);
					plot1->xAxis->setRange(widgets[*widget]->xAxis->range());
					plot1->yAxis->setRange(widgets[*widget]->yAxis->range());
					plot1->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding));
					plot1->setMinimumSize(1220, 300);


					tabs->setWindowTitle(QString::fromStdString(world->SimulationEntities->at(*widget)->Name));
					tabs->addTab(plot1, "Graph");
					tabs->addTab(plot, "Fourier Transform");
					tabs->setMinimumSize(1220, 300);
					tabs->show();
				}
				if (ADown)
				{
					tabs->clear();
					plot->clearItems();
					for (int k = 0; k < plot->graphCount(); k++)
						plot->graph(k)->clearData();
					plot->clearGraphs();
					QCustomPlot *plot1 = new QCustomPlot();
					bool temp = true;
					for (int k = 0; k < widgets[*widget]->graphCount(); k++)
					{
						if (2 * (k + 1) > plot->graphCount() - 1)
						{
							plot->addGraph();
							plot->addGraph();
						}
						temp = PlotFFT(*widget, k, 2 * k);
						plot1->addGraph();
						plot1->graph(k)->setData(widgets.at(*widget)->graph(k)->data(), true);
						plot1->graph(k)->setPen(widgets[*widget]->graph(k)->pen());
						plot1->graph(k)->setName(widgets[*widget]->graph(k)->name());

					}
					plot->replot();
					plot1->replot();
					plot1->setInteraction(QCP::Interaction::iRangeZoom, true);
					plot1->setInteraction(QCP::Interaction::iRangeDrag, true);
					plot1->setInteraction(QCP::Interaction::iSelectAxes, true);

					plot1->xAxis->setLabel("Time");
					plot1->yAxis->setLabel("Value");

					plot1->xAxis->setRange(widgets[*widget]->xAxis->range());
					plot1->yAxis->setRange(widgets[*widget]->yAxis->range());
					plot1->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding));
					plot1->setMinimumSize(1220, 300);
					tabs->setWindowTitle(QString::fromStdString(world->SimulationEntities->at(*widget)->Name));
					tabs->addTab(plot1, "Graph");
					tabs->addTab(plot, "Fourier Transform");
					tabs->setMinimumSize(1220, 300);
					tabs->show();

				}
			}
			else if (graph)
			{
				double key = 0;
				double value = 0;

				bool ok = false;
				double m = DBL_MAX;

				double d;
				int k = 0;
				foreach(QCPData data, graph->data()->values())
				{
					d = qAbs(x - data.key);

					if (d < m)
					{
						key = data.key;
						value = data.value;

						ok = true;
						m = d;
					}
				}

				if (ok)
				{
					QToolTip::showText(event->globalPos(),
						tr("<table>"
						"<tr>"
						"<th colspan=\"2\">%L1</th>"
						"</tr>"
						"<tr>"
						"<td>Time:</td>" "<td>%L2</td>"
						"</tr>"
						"<tr>"
						"<td>Value:</td>" "<td>%L3</td>"
						"</tr>"
						"</table>").
						arg(graph->name().isEmpty() ? "..." : graph->name()).
						arg(key).
						arg(value),
						temp, temp->rect());

				}

			}

			Check = false;
		}
		Check = false;
	}
	//evt.clear();
}
void Interop::plotReleaseEvent(QMouseEvent *event)
{
	QToolTip::hideText();
	if (HasDebugged)
		Check = true;
}