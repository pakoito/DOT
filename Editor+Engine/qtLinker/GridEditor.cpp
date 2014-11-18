#include "GridEditor.h"
#include <qcombobox.h>

void GridEditor::paintEvent(QPaintEvent * event)
{
	if (view->c == true)
	{
		TypeIndex typeindex = grid->CheckIntercept(view->Temp); //Check to see if we interecepted any of the ads or entites
		if (typeindex.t == -1)
		{
			pieMenu->showMenu();
		}
		view->c = false;
	}
	if (view->Delete == true)
	{
		Delete();
		view->Delete = false;
	}
	if (worldptr->ExternUpdate)
	{
		DeleteUnpaired();
		worldptr->ExternUpdate = false;
	}
	setFixedHeight(p->height() - 30);
}
void GridEditor::CreateEntity()
{
	widget = new InheritanceWidget(worldptr,0);
	connect(widget->button, SIGNAL(pressed()), SLOT(TextReturn()));
	widget->show();
	WhileSync = true;
}
void GridEditor::CreateAdvertisement()
{
	widget = new InheritanceWidget(worldptr, 1);
	connect(widget->button, SIGNAL(pressed()), SLOT(TextReturn()));
	widget->show();
	WhileSync = true;
}
