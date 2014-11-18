#ifndef GE_H
#define GE_H
#include <QWidget>
#include <QList>
#include <QVariant>
#include <QVector>
#include <QtWidgets>
#include "../AI_Sample/AILink.h"
#include "runner.h"
#include "TextBoxWidget.h"
#include "TreeViewEntity.h"
#include "SliderBoxWidget.h"
#ifndef LINK_STRUCT
#define LINK_STRUCT
struct Link
{
	TextBoxWidget *txt;
	void *L;
	int t;
	enum type
	{
		BOOL, INT, FLOAT, DOUBLE, STRING,
	};
};
#endif
class GradientEditor2DAdvanced : public QWidget
{
	Q_OBJECT
public:
	explicit GradientEditor2DAdvanced(World *wo, Gradient *grad, QWidget *parent = 0);
	explicit GradientEditor2DAdvanced(QWidget *parent = 0);

	World *link;
	Gradient *g;
	void SetUpGraph()
	{
		QVector<double> Yval;
		QVector<double> XVal;
		Gradient *ea = new Gradient();
		ea->Low = textboxes[0].txt->Title2->text().toInt();
		ea->Q1 = textboxes[1].txt->Title2->text().toInt();
		ea->Q3 = textboxes[2].txt->Title2->text().toInt();
		ea->Max = textboxes[3].txt->Title2->text().toInt();

		int L = sbw->Title2->value();
		float Lf = (float)L / 100;
		std::string temp = std::to_string(Lf);
		sbw->Title->setText("Z: " + QString::fromStdString(temp));
		int CurrentValue = textboxes[4].txt->Title2->text().toInt();
		XVal.push_back(0);
		Yval.push_back(1);
		for (double i = 1; i < 100; i++)
		{
			XVal.push_back(i/100);
			Yval.push_back((double)Monitization::AutoWeightedScore_Negative(i, Lf, i, CurrentValue, *ea));
		}
		graph->graph(0)->setData(XVal, Yval);

	}
	void newTB(QString n, void *L, int type = Link::type::INT)
	{
		Link temp;
		TextBoxWidget *q = new TextBoxWidget(n, StringFromVoid(L).c_str(), this);
		temp.txt = q;
		temp.L = L;
		temp.t = type;
		textboxes.push_back(temp);
		vLayout->addWidget(textboxes[textboxes.size() - 1].txt, 0, Qt::AlignTop);
	}
	std::string StringFromVoid(void *L)
	{
		std::string *sp = static_cast<std::string*>(L);
		try
		{
			sp->length();
			return *sp;
		}
		catch (exception e)
		{
			return std::string("NULL");
		}


	};

	QVector<Link> textboxes;
	QVBoxLayout *vLayout;
	bool GraphOrConfirm;
	bool AorS;
	QPushButton *m_button;
	QCustomPlot *graph;
	QWidget *p;
	QTabWidget *tabs;
	SliderBoxWidget *sbw;
	private slots:
	void TextChanged()
	{
		SetUpGraph();
		graph->replot();
		//this->GraphOrConfirm = false;
		//m_button->setText("Graph?");
		//update();
	}
	void GraphButtonPress()
	{
		Gradient *ea = new Gradient();
		ea->Low = textboxes[0].txt->Title2->text().toInt();
		ea->Q1 = textboxes[1].txt->Title2->text().toInt();
		ea->Q3 = textboxes[2].txt->Title2->text().toInt();
		ea->Max = textboxes[3].txt->Title2->text().toInt();

		*g = *ea;
	};

};
#endif