#include "MainWindow.h"
#include "qtlinker.h"
#include <QtWidgets/QApplication>
#include <QPixmap>
#include <QSplashScreen>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QPixmap pixmap("DOT_Logo.png"); //Insert your splash page image here
	if (pixmap.isNull())
	{
		QMessageBox::warning(0, "Error", "Failed to load Splash Screen image!");
	}
	
	QSplashScreen splash(pixmap, Qt::WindowFlags(Qt::WindowType::SplashScreen));
	splash.setMaximumSize(QSize(452, 800));
	splash.show();
	splash.showMessage("Loading last save and compiling. Please wait...", Qt::AlignRight | Qt::AlignTop, Qt::black);
	a.processEvents();

	QFile File("QTDark.stylesheet");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(File.readAll());


	a.setStyleSheet(StyleSheet);



	Window w;
	w.adjustSize();
	XMLHandler handl = XMLHandler("temp.xml", w.interop->ReturnWorld(), w.interop->ReturnGrid()->grid, false);
	w.show();
	splash.finish(&w);
    a.exec();
	handl = XMLHandler("temp.xml", w.interop->ReturnWorld(), w.interop->ReturnGrid()->grid, true);
	return 0;
}
