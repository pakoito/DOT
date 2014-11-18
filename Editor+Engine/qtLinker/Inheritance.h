#ifndef I_H
#define I_H
#include <QWidget>
#include <QtWidgets>
#include <qcombobox.h>
#include "../AI_Sample/AILink.h"
#include "runner.h"

class InheritanceWidget : public QWidget
{
	Q_OBJECT
public:
	InheritanceWidget(World *worldptr, int type, QWidget *parent = 0)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
		setWindowTitle("Polymorphism");
		vLayout = new QHBoxLayout(this);
		QLineEdit *Title = new QLineEdit("Inherits from: ");
		Title->setMaximumWidth(100);
		Title->setDisabled(true);
		vLayout->addWidget(Title);
		combobox = new QComboBox();
		combobox->setMinimumWidth(250);
		t = type;
		switch (type)
		{
		case 0:
			for (int i = 0; i < worldptr->Entities->size(); i++)
			{
				std::string Name = worldptr->Entities->at(i).get()->Name;
				combobox->addItem(QString::fromStdString(Name));
			}
			break;
		case 1:
			for (int i = 0; i < worldptr->Ads->size(); i++)
			{
				combobox->addItem(QString::fromStdString(worldptr->Ads->at(i).get()->Name));
			}
			break;
		default:
			break;
		}

		vLayout->addWidget(combobox);
		button = new QPushButton("Create");
		button->setMinimumWidth(90);
		vLayout->addWidget(button);
		setFixedWidth(500);
		setFixedHeight(45);
	};
	InheritanceWidget(QLineEdit *Title, QWidget *parent = 0)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
		setWindowTitle("Polymorphism");
		vLayout = new QHBoxLayout(this);
		Title->setMaximumWidth(100);
		Title->setDisabled(true);
		vLayout->addWidget(Title);
		line = new QLineEdit();
		line->setMinimumWidth(250);


		vLayout->addWidget(line);
		button = new QPushButton("Create");
		button->setMinimumWidth(90);
		vLayout->addWidget(button);
		QClipboard *cb = QApplication::clipboard();
		cb->clear();
		setFixedWidth(500);
		setFixedHeight(45);
		init();
	};
	~InheritanceWidget()
	{
		_sleep(10);
	}
	int t;
	QWidget *p;
	QPushButton *button;
	QComboBox *combobox;
	QLineEdit *line;

private:
	void init(){
		setAcceptDrops(false);
		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(line, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showMenu(QPoint)));
	}
protected:

	private slots:
	void showMenu(QPoint position){}
private:
	virtual bool eventFilter(QObject *obj, QEvent *event)
	{
		if (obj == line) {
			if (event->type() == QEvent::KeyPress) {
				QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
				if (keyEvent->matches(QKeySequence::Copy) || keyEvent->matches(QKeySequence::Cut) || keyEvent->matches(QKeySequence::Paste))
				{
					event->ignore();
					return true;
				}
			}
			// pass the event on to the parent class
			return line->eventFilter(obj, event);
		}
		return true;
	};
	QHBoxLayout *vLayout;
};
#endif