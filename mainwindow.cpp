#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createAndFillmdiArea();
}

MainWindow::~MainWindow()
{

}
void MainWindow::createAndFillmdiArea(){
    mdiArea=new QMdiArea(this);
    this->setCentralWidget(mdiArea);
    mdiArea->setViewMode(QMdiArea::TabbedView);

    QMenuBar * mainMenu=new QMenuBar(this);
    this->setMenuBar(mainMenu);

    mainMenu->addAction("Settings",this,SLOT(action_open_Settings()));
}

void MainWindow::action_open_Settings(){
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->widget()->objectName()=="Settings"){
            x->close();
        };
    };
    QMdiSubWindow *subWindow1 = new QMdiSubWindow(this);
    Settings_gui* settings=new Settings_gui(this);
    subWindow1->setWidget(settings);
    subWindow1->setWindowTitle("Settings");
    mdiArea->addSubWindow(subWindow1);
    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
    subWindow1->show();
}
