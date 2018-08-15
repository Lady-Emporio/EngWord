#include "mdi_dl.h"



MDi_DL::MDi_DL(QWidget *parent,QString parent_id,QString parentTableName)
   : QWidget(parent),parent_id(parent_id),parentTableName(parentTableName)
{
    This_settings=S::Settings()->All_QString_PARAMS;
    MakeGui();
    AddDlLists();
}

void MDi_DL::MakeGui()
{
    mdiArea=new QMdiArea(this);
    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(mdiArea);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::North);
    mdiArea->setTabShape(QTabWidget::Triangular);
    //mdiArea->setTabsMovable(false);
    this->setLayout(mainLayout);

}

void MDi_DL::AddDlLists()
{
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        x->close();
    };
    if(parentTableName==This_settings["EngWordTable"]){
        QMdiSubWindow *subWindow1 = new QMdiSubWindow(this);
        subWindow1->setWindowTitle("Dl "+This_settings["EngTranslateTable"]);
        DynamicList * EngTranslateTable=new DynamicList(this,This_settings["EngTranslateTable"],"",parent_id);
        subWindow1->setWidget(EngTranslateTable);
        mdiArea->addSubWindow(subWindow1);
        subWindow1->setAttribute(Qt::WA_DeleteOnClose);
        subWindow1->showFullScreen();


        connect(EngTranslateTable, SIGNAL(OpenObject(QString, QString)),(QObject*)S::Settings()->MyGlobalApp, SLOT(OpenObject(QString, QString)));
        connect(EngTranslateTable, SIGNAL(CreateObject(QString, QString)), (QObject*)S::Settings()->MyGlobalApp, SLOT(CreateObject(QString, QString)));
    }else if(parentTableName==This_settings["EngTranslateTable"]){
        QMdiSubWindow *subWindow2 = new QMdiSubWindow(this);
        subWindow2->setWindowTitle("Dl "+This_settings["RuTranslateTable"]);
        DynamicList * RuTranslateTable=new DynamicList(this,This_settings["RuTranslateTable"],"",parent_id);
        subWindow2->setWidget(RuTranslateTable);
        mdiArea->addSubWindow(subWindow2);
        subWindow2->setAttribute(Qt::WA_DeleteOnClose);
        subWindow2->show();

        QMdiSubWindow *subWindow3 = new QMdiSubWindow(this);
        subWindow3->setWindowTitle("Dl "+This_settings["exampleTable"]);
        DynamicList * exampleTable=new DynamicList(this,This_settings["exampleTable"],"",parent_id);
        subWindow3->setWidget(exampleTable);
        mdiArea->addSubWindow(subWindow3);
        subWindow3->setAttribute(Qt::WA_DeleteOnClose);
        subWindow3->show();

        connect(RuTranslateTable, SIGNAL(OpenObject(QString, QString)), (QObject*)S::Settings()->MyGlobalApp, SLOT(OpenObject(QString, QString)));
        connect(exampleTable, SIGNAL(OpenObject(QString, QString)),     (QObject*)S::Settings()->MyGlobalApp, SLOT(OpenObject(QString, QString)));
        connect(RuTranslateTable, SIGNAL(CreateObject(QString, QString)), (QObject*)S::Settings()->MyGlobalApp, SLOT(CreateObject(QString, QString)));
        connect(exampleTable, SIGNAL(CreateObject(QString, QString)), (QObject*)S::Settings()->MyGlobalApp, SLOT(CreateObject(QString, QString)));
    }
}
