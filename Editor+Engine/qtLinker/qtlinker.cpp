#include "qtlinker.h"

Window::Window(QWidget *parent) :
QWidget(parent)
{
    setWindowTitle(tr("Display"));
	resize(1280, 960);
    interop = new MainWindow(this);
}