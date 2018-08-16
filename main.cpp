#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    S::Settings();
    MainWindow *w=new MainWindow;
    S::Settings()->MyGlobalApp=w;
    a.setWindowIcon(QIcon(S::get("path/to/image/x.png")));
    w->show();
    return a.exec();
}
