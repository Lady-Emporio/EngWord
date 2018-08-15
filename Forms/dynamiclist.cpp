#include "dynamiclist.h"
#include "separateclearfunc.h"
DynamicList::DynamicList(QWidget *parent,QString table_name,QString mark_del,QString This_is_parents)
    : QWidget(parent),table_name(table_name),mark_del(mark_del),This_is_parents(This_is_parents)
{
    This_settings=S::Settings()->All_QString_PARAMS;
    if(table_name==This_settings["EngWordTable"]){
        parent_table="";
    }else if(table_name==This_settings["EngTranslateTable"]){
        parent_table=table_name==This_settings["EngWordTable"];
    }else if(table_name==This_settings["RuTranslateTable"] || table_name==This_settings["exampleTable"]){
        parent_table=table_name==This_settings["EngTranslateTable"];
    };
    makeGui();
    readFromDB();
}

void DynamicList::readFromDB()
{
    dynamic_list->clear();

    QString transaction_name="read_list";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QString sql;
    if(This_is_parents!=""){
        sql="SELECT presentation,id,del_mark from "+table_name+" where parent="+This_is_parents+";";
    }else{
        sql="SELECT presentation,id from "+table_name+" where del_mark="+mark_del+";";
    }
    if(!query.exec(sql)){
        GetErrorMessage(&db,transaction_name,&query);
        return;
    };
    while(query.next()){
        QString Its_presentation=query.value(0).toString();
         if(This_is_parents!=""){
             QString its_del_mark=query.value(2).toString();
             if(its_del_mark=="0"){
                Its_presentation+=" delete";
             }
         }
        QString Its_id=query.value(1).toString();
        QListWidgetItem * nextLW=new QListWidgetItem(Its_presentation,dynamic_list);
        nextLW->setWhatsThis(Its_id+"|"+table_name);
        dynamic_list->insertItem(dynamic_list->count()+1,nextLW);

    };
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
}

void DynamicList::open()
{
    QListWidgetItem* active=dynamic_list->currentItem();
    if(active!=nullptr){
        QStringList data=active->whatsThis().split("|");
        QString id=data[0];
        QString table=data[1];
        emit OpenObject(id,table);
    }
}

void DynamicList::create()
{
    QListWidgetItem* active=dynamic_list->currentItem();
    if(active!=nullptr){
        QStringList data=active->whatsThis().split("|");
        QString id=data[0];
        QAction *act = qobject_cast<QAction *>(sender());
        QString table_name=act->data().toString();
        QString table=data[1];
        qDebug()<< table_name<<id<<"wish create";
        emit CreateObject(table_name,id);
    }else{
        emit CreateObject(This_settings["EngWordTable"]);
    }
}

void DynamicList::makeGui()
{
    QVBoxLayout * mainLayout=new QVBoxLayout(this);
    this->setLayout(mainLayout);


    QMenuBar * mainMenu = new QMenuBar;
    if(table_name==This_settings["EngWordTable"]){
        QMenu * create=new QMenu("Create",this);
            QAction * act1=create->addAction(This_settings["EngWordTable"],this,SLOT(create()));
            act1->setData(This_settings["EngWordTable"]);

            QAction * act2=create->addAction(This_settings["EngTranslateTable"],this,SLOT(create()));
            act2->setData(This_settings["EngTranslateTable"]);
        mainMenu->addMenu(create);
    }
    else if(table_name==This_settings["EngTranslateTable"]){
        QMenu * create=new QMenu("Create",this);
        QAction * act1=create->addAction(This_settings["RuTranslateTable"],this,SLOT(create()));
        act1->setData(This_settings["RuTranslateTable"]);
        QAction * act2=create->addAction(This_settings["exampleTable"],this,SLOT(create()));
        act2->setData(This_settings["exampleTable"]);
        mainMenu->addMenu(create);
    };
    QPushButton * refresh=new QPushButton("Refresh",this);

    mainLayout->setMenuBar(mainMenu);
    mainMenu->addAction("open",this,SLOT(open()));
    dynamic_list=new QListWidget(this);
    mainLayout->addWidget(dynamic_list);
    mainLayout->addWidget(refresh);

    connect(refresh, SIGNAL(clicked()), this, SLOT(readFromDB()));
}
