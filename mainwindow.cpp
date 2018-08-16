#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QMenuBar * mainMenu=new QMenuBar(this);
    this->setMenuBar(mainMenu);

    QMenu * AllDynamicList=new QMenu("All DynamicList",this);
    QStringList tables;
    QString wordTable=S::get("EngWordTable");
    tables<<wordTable
          <<S::get("EngTranslateTable")
          <<S::get("RuTranslateTable")
          <<S::get("exampleTable");
    for(int mark=0;mark!=2;++mark){
        QString mark_del=QString("").setNum(mark);
        for(int i=0;i!=tables.length();++i){
            QString name_act=(mark_del=="1"?"астивные":"помеченные на удаление");
            QAction * act=AllDynamicList->addAction(tables[i]+" "+name_act,this,SLOT(action_Dynamic_EngTable()));
            act->setData(tables[i]+"|"+mark_del);
        }
    }

    mdiArea = new QMdiArea(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //mdiArea->setViewMode(QMdiArea::TabbedView);
    setCentralWidget(mdiArea);

    QAction * act=mainMenu->addAction("Word",this,SLOT(action_Dynamic_EngTable()));
    act->setData(wordTable+"|"+"1");



    mainMenu->addAction("Settings",this,SLOT(action_Settings()));
    mainMenu->addMenu(AllDynamicList);
    mainMenu->addAction("SelectWord",this,SLOT(action_SelectWord()));
    mainMenu->addAction("InputWrite",this,SLOT(action_InputWrite()));
    //FormObject *widget=new FormObject(this,S::get("EngWordTable"));//добавление в таблицу word новое значение
    //FormObject *widget=new FormObject(this,S::get("RuTranslateTable"),"","979");// добавление нового в другую таблицу

    //FormObject *widget=new FormObject(this,S::get("RuTranslateTable"),"6");//  открытие
    //FormObject *widget=new FormObject(this,S::get("EngWordTable"),"4");//  открытие

//    DynamicList * widget=new DynamicList(this,S::get("EngWordTable"));
//    this->setCentralWidget(widget);
//
}

MainWindow::~MainWindow()
{

}

void MainWindow::OpenObject(QString id, QString table)
{
    QString title=table+":"+id;
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    FormObject *widget=new FormObject(this,table,id);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
    connect(widget, SIGNAL(needOpenDL(QString,QString)), this, SLOT(needOpenDL(QString,QString)));
    connect(widget, SIGNAL(OpenObject(QString,QString)), this, SLOT(OpenObject(QString,QString)));
//    if(table==S::get("EngWordTable")){
//        connect(widget, SIGNAL(needOpenDL(QString,QString)), this, SLOT(needOpenDL(QString,QString)));
//    }else{
//        connect(widget, SIGNAL(OpenObject(QString,QString)), this, SLOT(OpenObject(QString,QString)));
//    }
}

void MainWindow::action_Settings()
{
    QString title="Settings";
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    Settings_gui * widget=new Settings_gui(this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
}

void MainWindow::action_Dynamic_EngTable()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QString data=act->data().toString();
    QStringList data_list=data.split("|");
    QString table_name=data_list[0];
    QString mark_del=data_list[1];

    QString name_act=(mark_del=="1"?"астивные":"помеченные на удаление");
    QString title=table_name+" "+name_act;


    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    DynamicList * widget=new DynamicList(this,table_name,mark_del);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();

    connect(widget, SIGNAL(OpenObject(QString, QString)), this, SLOT(OpenObject(QString, QString)));
    connect(widget, SIGNAL(CreateObject(QString, QString)), this, SLOT(CreateObject(QString, QString)));
}

void MainWindow::CreateObject(QString table,QString parent_id)
{
    QString title;
    FormObject *widget;
    if(table==S::get("EngWordTable")){
        widget=new FormObject(this,table);
        title="Create new word";
        connect(widget, SIGNAL(OpenNowCreated(FormObject*,QString,QString)), this, SLOT(OpenNowCreated(FormObject*,QString,QString)));
    }else{
        widget=new FormObject(this,table,"",parent_id);
        title="Create new "+table;
        connect(widget, SIGNAL(OpenObject(QString,QString)), this, SLOT(OpenObject(QString,QString)));
    }
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);

    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
}

void MainWindow::needOpenDL(QString parent, QString table_name)
{
    QString title="Childrens "+parent;
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };

    if(table_name==S::Settings()->All_QString_PARAMS["EngWordTable"]){
        QMdiSubWindow *subWindow = new QMdiSubWindow(this);
        subWindow->setWindowTitle(title);
        DynamicList * widget=new DynamicList(this,S::Settings()->All_QString_PARAMS["EngTranslateTable"],"",parent);
        subWindow->setWidget(widget);
        mdiArea->addSubWindow(subWindow);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        subWindow->show();

        connect(widget, SIGNAL(OpenObject(QString, QString)),this, SLOT(OpenObject(QString, QString)));
        connect(widget, SIGNAL(CreateObject(QString, QString)), this, SLOT(CreateObject(QString, QString)));
    }else{
        QMdiSubWindow *subWindow = new QMdiSubWindow(this);
        subWindow->setWindowTitle(title);
        MDi_DL* widget=new MDi_DL(this,parent,table_name);
        subWindow->setWidget(widget);
        mdiArea->addSubWindow(subWindow);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        subWindow->show();

        subWindow->resize(300,200);
    }
}

void MainWindow::OpenNowCreated(FormObject *needClose, QString table_name, QString id)
{
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    QPoint pos;
    for(auto x:allSub){
        if(x->widget()==needClose){
            pos=x->pos();
            x->close();
        };
    };
    QString title=table_name+":"+id;

    QMdiSubWindow *subWindow = new QMdiSubWindow(this);
    subWindow->setWindowTitle(title);
    //qDebug()<<table<<id;
    FormObject *widget=new FormObject(this,table_name,id);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();

    connect(widget, SIGNAL(needOpenDL(QString,QString)), this, SLOT(needOpenDL(QString,QString)));
    subWindow->move(pos);
}

void MainWindow::action_SelectWord()
{
    QString title="SelectWord";
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);

    subWindow->setWindowTitle(title);

    SelectWord * widget=new SelectWord(this);
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
}

void MainWindow::action_InputWrite()
{
    QString title="InputWrite";
    QList<QMdiSubWindow *>	allSub=mdiArea->subWindowList();
    for(auto x:allSub){
        if(x->windowTitle()==title){
            x->close();
        };
    };
    QMdiSubWindow *subWindow = new QMdiSubWindow(this);

    subWindow->setWindowTitle(title);

    InputWrite *inputWrite=new InputWrite(this);

    S::Settings()->InpWr=inputWrite;
    inputWrite->startWithHook();

    subWindow->setWidget(inputWrite);
    mdiArea->addSubWindow(subWindow);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->show();
}
