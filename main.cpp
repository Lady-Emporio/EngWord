#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    S::Settings();
//    MainWindow w;
//    w.show();
    MainWindow *w=new MainWindow;
    S::Settings()->MyGlobalApp=w;
    w->show();
    return a.exec();
}
