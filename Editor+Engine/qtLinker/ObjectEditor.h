#ifndef OE_H
#define OE_H
#include <QWidget>
#include <QList>
#include <QVariant>
#include <QVector>
#include <QtWidgets>
#include "../AI_Sample/AILink.h"
#include "runner.h"
#include "TextBoxWidget.h"
#include "TreeViewEntity.h"

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
class ObjectEditor : public QWidget
{
	Q_OBJECT
public:
	explicit ObjectEditor(World *wo, QWidget *parent = 0);
	explicit ObjectEditor(QWidget *parent = 0);
	~ObjectEditor();
	int w, h, f;
	QWidget *p;
	void CostShow(Cost *in);
	void EntityShow(Entity *in);
	void AdvertisementShow(Advertisement *in);
	void AttributeShow(EntityAttribute *in);
	void GradShow(Gradient *in);
	QHBoxLayout *ParentLayout;

	bool Activated;


	std::shared_ptr<Entity> LinkECopy;
	std::shared_ptr<Advertisement> LinkACopy;
	std::shared_ptr<EntityAttribute> LinkEACopy;
	std::shared_ptr<Cost> LinkCoCopy;

	bool MasterChild;
	QModelIndex indexof;
	ObjectEditor *parentob;
	class TreeWidget *wid;
private:
	void newTB(QString n, void *L, int type = Link::type::INT)
	{
		Link temp;
		TextBoxWidget *q = new TextBoxWidget(n, QString::fromStdString(StringFromVoid(L)), this);
		temp.txt = q;
		temp.L = L;
		temp.t = type;
		textboxes.push_back(temp);
		vLayout->addWidget(textboxes[textboxes.size() - 1].txt, 0, Qt::AlignTop);
	}
	void newTB(QString n, QString string, int type = Link::type::INT)
	{
		Link temp;
		TextBoxWidget *q = new TextBoxWidget(n, string, this);
		temp.txt = q;
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
	void Clear()
	{

		textboxes.clear();
	}
	void keyPressEvent(QKeyEvent *event)
	{
		evt.push_back(event->key());
	}
	void keyReleaseEvent(QKeyEvent *keyevent)
	{
		for (int i = 0; i < evt.size(); i++)
		{
			if (evt.at(i) == keyevent->key())
			{
				evt.erase(evt.begin() + i);
			}
		}
	}
	QVector<int> evt;
	QVBoxLayout *vLayout;
	World *worldptr;
	QVector<Link> textboxes;
	void paintEvent(QPaintEvent * event);
	TreeWindowEntity *tree;
	ObjectEditor *ob;
	int last;
	int typealloc;
	//LINKS:

	Advertisement *ad;
	Entity *e;
	EntityAttribute *ea;
	Cost *co;
	Gradient *ga;
	QPushButton *m_button;
	QTreeView *view;
	int InUse;
	private slots:
	void OnClick(const QModelIndex&);
	void OnButton();
	void hideEvent(QHideEvent *event);
	void CreateNew();
};
#endif