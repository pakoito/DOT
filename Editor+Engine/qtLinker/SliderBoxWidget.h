#ifndef SBW_H
#define SBW_H
#include <QWidget>
#include <QtWidgets>
#include "../AI_Sample/AILink.h"
#include "runner.h"

class SliderBoxWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SliderBoxWidget(QString Title = "default", int val = 0, QWidget *parent = 0);
	~SliderBoxWidget();
	int w, h, f;
	QWidget *p;
	QSlider *Title2;
	QLineEdit *Title;
private:

	QHBoxLayout *vLayout;
	void paintEvent(QPaintEvent * event);
};
#endif