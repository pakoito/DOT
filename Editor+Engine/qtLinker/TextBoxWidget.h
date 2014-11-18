#ifndef TBW_H
#define TBW_H
#include <QWidget>
#include <QtWidgets>
#include "../AI_Sample/AILink.h"
#include "runner.h"

class TextBoxWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TextBoxWidget(QString Title = "default", QString val = "default", QWidget *parent = 0);
	~TextBoxWidget();
	int w, h, f;
	QWidget *p;
	QLineEdit *Title2;
private:

	QHBoxLayout *vLayout;
	void paintEvent(QPaintEvent * event);
};
#endif