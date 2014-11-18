#ifndef GC_H
#define GC_H
#include <qgraphicsscene.h>
#include <qpainter.h>
#include <qline.h>
#include <QDebug>
#include <qdatetime.h>
#include "runner.h"
struct AdEdit
{
public:
	int index;
	std::string Name;
	VECTOR2DF Position;
	QRect *rect;
	std::string Link;

};
struct EntityEdit
{
public:
	int index;
	std::string Name;
	VECTOR2DF Position;
	QRect *rect;
	std::string Link;
};
struct TypeIndex
{
	int t, i;
	TypeIndex(){};
	TypeIndex(int tn, int in)
	{
		t = tn;
		i = in;
	}
};
class GridScene : public QGraphicsScene
{
public:
	GridScene(qreal x, qreal y, qreal w, qreal h)
		: QGraphicsScene(x, y, w, h)
	{
		selected = TypeIndex(-1, -1);
	}
	TypeIndex selected;
	QVector<AdEdit> Ads;
	QVector<EntityEdit> Entities;
	TypeIndex CheckIntercept(QPoint temp)
	{
		for (int i = 0; i < Entities.size(); i++)
		{
			if (Entities.at(i).rect->contains(temp))
			{
				selected = TypeIndex(0, i);
				return selected;
			}
		}
		for (int i = 0; i < Ads.size(); i++)
		{
			if (Ads.at(i).rect->contains(temp))
			{
				selected = TypeIndex(1, i);
				return selected;
			}
		}
		return TypeIndex(-1, -1);
	};
	TypeIndex CheckIntercept_NoChange(QPoint temp)
	{
		for (int i = 0; i < Entities.size(); i++)
		{
			if (Entities.at(i).rect->contains(temp))
			{
				return TypeIndex(0, i);
			}
		}
		for (int i = 0; i < Ads.size(); i++)
		{
			if (Ads.at(i).rect->contains(temp))
			{
				return TypeIndex(1, i);
			}
		}
		return TypeIndex(-1, -1);
	};
	std::string ReturnName()
	{
		switch (selected.t)
		{
		case 0:
			return Entities.at(selected.i).Name;
			break;
		case 1:
			return Ads.at(selected.i).Name;
			break;
		default:
			return std::string("");
			break;
		}
	}
protected:
	void drawBackground(QPainter *painter, const QRectF &rect)
	{
		painter->setPen(Qt::darkGray);
		const int gridSize = 25;

		qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
		qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

		QVarLengthArray<QLineF, 100> lines;

		for (qreal x = left; x < rect.right(); x += gridSize)
			lines.append(QLineF(x, rect.top(), x, rect.bottom()));
		for (qreal y = top; y < rect.bottom(); y += gridSize)
			lines.append(QLineF(rect.left(), y, rect.right(), y));

		painter->drawLines(lines.data(), lines.size());
	}

	void drawForeground(QPainter *painter, const QRectF &rect)
	{
		painter->setBrush(Qt::BrushStyle::SolidPattern);

		switch (selected.t)
		{
		case 0: //An Entity was selected
			painter->setPen(Qt::red);
			for (int i = 0; i < Entities.size(); i++)
			{
				if (selected.i != i)
					painter->drawEllipse(*Entities.at(i).rect);
				else
				{
					painter->setPen(Qt::yellow);
					painter->drawEllipse(*Entities.at(i).rect);
					painter->setPen(Qt::red);
				}
			}
			painter->setPen(Qt::white);
			for (int i = 0; i < Ads.size(); i++)
			{
				painter->drawRect(*Ads.at(i).rect);
			}
			break;
		case 1: //An Ad was selected
			painter->setPen(Qt::red);
			for (int i = 0; i < Entities.size(); i++)
			{
				painter->drawEllipse(*Entities.at(i).rect);
			}
			painter->setPen(Qt::white);
			for (int i = 0; i < Ads.size(); i++)
			{
				if (selected.i != i)
					painter->drawRect(*Ads.at(i).rect);
				else
				{
					painter->setPen(Qt::yellow);
					painter->drawRect(*Ads.at(i).rect);
					painter->setPen(Qt::white);
				}

			}
			break;
		default://Nothing has been selected
			painter->setPen(Qt::red);
			for (int i = 0; i < Entities.size(); i++)
			{
				painter->drawEllipse(*Entities.at(i).rect);
			}
			painter->setPen(Qt::white);
			for (int i = 0; i < Ads.size(); i++)
			{
				painter->drawRect(*Ads.at(i).rect);
			}

			break;
		}

	}

};
#endif