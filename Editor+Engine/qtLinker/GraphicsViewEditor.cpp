#include "runner.h"
#include "GridScene.h"
#include <qwidget.h>
#include "qpiemenu.h" 
#include <qpoint.h>
#include "TextBoxWidget.h"
#include "Inheritance.h"
class GraphcisViewEditor : public QGraphicsView
{
public:
	GraphcisViewEditor(QGraphicsScene *scene, QWidget *parent = 0)
		: QGraphicsView(scene, parent)
	{
		b = false;
		c = false;
		Temp = QPoint(0, 0);
		Last = Temp;
		Control = false;
		zoomval = 0;
	}
	GraphcisViewEditor::~GraphcisViewEditor()
	{

	}
	bool b;
	bool c;
	bool Delete;
	QPoint Temp;
	QPoint Last;
	QWidget *p;
	GridScene *ptr;
	World *world;
	QPoint GetLocal(QPoint Window)
	{
		Window = mapFromParent(Window);
		Window = mapToScene(Window).toPoint();
		return Window;
	};
	void mousePressEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::RightButton)
		{
			if (this->rect().contains(event->windowPos().toPoint()))
			{
				c = true;

				Temp = event->localPos().toPoint();
				Temp = mapToScene(Temp).toPoint();
				p->repaint();
				if (ptr->Entities.size() > 0 || ptr->Ads.size() > 0)
				{
					if (ptr->CheckIntercept_NoChange(Temp).t != -1)
						QToolTip::showText(event->globalPos(), QString::fromStdString(ptr->ReturnName()), this);
				}
			}

		}		
		if (event->button() == Qt::LeftButton)
		{
			if (this->rect().contains(event->windowPos().toPoint()))
			{
				b = true;
				QToolTip::hideText();
				Temp = event->localPos().toPoint();
				Temp = mapToScene(Temp).toPoint();
				p->repaint();
			}
		}
	};
	void mouseReleaseEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton)
		{
			if (this->rect().contains(event->windowPos().toPoint()))
			{
				b = false;
				Temp = event->localPos().toPoint();
				Temp = mapToScene(Temp).toPoint();
			}
		}
	}
	void wheelEvent(QWheelEvent* event) {
		if (Control){
			b = false;
			this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
			// Scale the view / do the zoom
			double scaleFactor = 1.15;
			if (event->delta() > 0) {
				// Zoom in
				if (zoomval < 5)
				{
					this->scale(scaleFactor, scaleFactor);
					zoomval++;
				}
			}
			else {
				// Zooming out
				if (zoomval > -5)
				{
					this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
					zoomval--;
				}
			}
		}

		// Don't call superclass handler here
		// as wheel is normally used for moving scrollbars
	}
	void mouseMoveEvent(QMouseEvent *event)
	{
		Last = Temp;
		Temp = event->localPos().toPoint();
		Temp = mapToScene(Temp).toPoint();

		if (ptr->selected.t != -1)
		if (b) //We've left clicked, and now we've moving the mouse
		{

			VECTOR2DF vec;
			QRect *rect;
			switch (ptr->selected.t)
			{
			case 0: //We've moving an Entity
				rect = ptr->Entities.at(ptr->selected.i).rect;
				vec = world->ReturnSimulationalEntity(ptr->Entities.at(ptr->selected.i).Name)->Position;
				break;
			case 1: //We've moving an Ad
				rect = ptr->Ads.at(ptr->selected.i).rect;
				vec = world->ReturnSimulationalAdvertisement(ptr->Ads.at(ptr->selected.i).Name)->Position;
				break;
			default:
				break;
			}
			rect->translate(Temp.x() - Last.x(), Temp.y() - Last.y());
			vec.Add(VECTOR2DF(Temp.x() - Last.x(), Temp.y() - Last.y()));

			switch (ptr->selected.t)
			{
			case 0: //We've moving an Entity
				world->ReturnSimulationalEntity(ptr->Entities.at(ptr->selected.i).Name)->Position = vec;
				break;
			case 1: //We've moving an Ad
				world->ReturnSimulationalAdvertisement(ptr->Ads.at(ptr->selected.i).Name)->Position =vec;
				break;
			default:
				break;
			}
			repaint();
			p->repaint(); 
		}
	};
	void keyPressEvent(QKeyEvent *keyevent)
	{
		if (keyevent->key() == Qt::Key_Delete)
		{
			Delete = true;
			p->repaint();
		}
		else if (keyevent->key() == Qt::Key_Control)
		{
			b = false;
			Control = true;
		}
	}
	void keyReleaseEvent(QKeyEvent *keyevent)
	{
		if (keyevent->key() == Qt::Key_Control)
		{
			b = false;
			Control = false;
		}
	}
	int zoomval;
	bool Control;

};
