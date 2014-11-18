#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <qwidget.h>
#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QAction>
#include <qlayout.h>
#include "../qtLinker/runner.h"
#include "../qtLinker/WriteXML.h"
class SideBar : public QWidget
{
    Q_OBJECT

public:
	SideBar(QHBoxLayout *layout, QWidget *parent = 0);
    ~SideBar();

	void addAction(QAction *action, QWidget* link, const Qt::AlignmentFlag Align = (Qt::AlignmentFlag::AlignLeading));
	QAction *addAction(const QString &text, QWidget* link, const Qt::AlignmentFlag Align = (Qt::AlignmentFlag::AlignLeading), const QIcon &icon = QIcon());
	QList<QAction*> _actions;
	QList<QWidget*> _widgets;
	QList<Qt::AlignmentFlag> Alignment;
	QWidget* returnactivewidget()
	{
		return _widgets.at(active);
	};
	int PresentWidget()
	{
		if (_actions.at(active)->text() == "Debug")
		{
			Interop *in;
			in = new Interop(world->SimulationEntities->size(), world);
			in->p = p;
			in->scene = gs;
			_widgets[active] = in;
		}
		if (_actions.at(active)->text() == "To XML")
		{
			QString filename = QFileDialog::getSaveFileName(this,
				tr("Save Xml"), ".",
				tr("Xml files (*.xml)"));
			XMLHandler *h = new XMLHandler(filename, world);
			Hide();
			return 0;
		}
		if (_actions.at(active)->text() == "From XML")
		{
			QString filename = QFileDialog::getOpenFileName(this,
				tr("Open Xml"), ".",
				tr("Xml files (*.xml)"));
			XMLHandler *h = new XMLHandler(filename, world, gs,false);
			Hide();
			return 0;
		}
		RefLayout->addWidget(_widgets.at(active), 0, Alignment.at(active));
		_widgets.at(active)->show();
		return 1;
	}
	void Hide()
	{
		if (_widgets.size() > 0)
			for (int i = 0; i < _widgets.size(); i++)
			{
				_widgets.at(i)->hide();
			}
		if (_checkedAction)
			_checkedAction->setChecked(false);
		active = -1;
	}
	void HideChildren()
	{
		foreach(SideBar *obj, _widgets.at(active)->findChildren<SideBar*>())
		{
			obj->setVisible(false);
		}
	};
	QAction *_pressedAction;
	QAction *_checkedAction;
	QHBoxLayout *RefLayout;
	QWidget *p;
	GridScene *gs;
	int active;
	World *world;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void hideEvent(QHideEvent *event);
    QSize minimumSizeHint() const;

    QAction* actionAt(const QPoint &at);
signals:
	void Pressed();

};

#endif // SIDEBAR_H
