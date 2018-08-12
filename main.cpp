#include "mainwindow.h"
#include "Settings/s.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    S::Settings();
    a.setWindowIcon(QIcon(S::get("path/to/image")));
    MainWindow w;
    w.show();

    return a.exec();
}
