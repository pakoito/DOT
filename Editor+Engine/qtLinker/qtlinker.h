#ifndef QTLINKER_H
#define QTLINKER_H

#include <QWidget>
#include "runner.h"
#include "MainWindow.h"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = 0);
	MainWindow* interop;

signals:

    public slots :

};

#endif // WINDOW_H
