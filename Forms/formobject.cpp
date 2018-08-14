#include "formobject.h"
#include "Settings/s.h"
#include "separateclearfunc.h"
FormObject::FormObject(QWidget *parent,QString table_name,QString id,QString parent_id)
    : QWidget(parent),table_name(table_name),id(id),parent_id(parent_id)
{
    This_settings=S::Settings()->All_QString_PARAMS;
    makeDefaultGui();
    if(id!=""){
        openExist();
    }else{
        createNew();
    }
}
void FormObject::makeDefaultGui(){
    presentation=new QLabel(this);
    edit=new QLineEdit(this);
    create=new QPushButton("Create",this);
    mark_delete=new QPushButton("Mark to delete",this);
    QGridLayout  *mainLayout=new QGridLayout (this);
    this->setLayout(mainLayout);
    parent=new QLabel("Parent",this);
    parent_buttons=new QPushButton(parent_id,this);

    openAll=new QPushButton("open children lists",this);
    openAll->hide();
    mainLayout->addWidget(openAll        ,0,0,1,4,Qt::AlignLeft);

    mainLayout->addWidget(parent        ,0,0,1,2,Qt::AlignLeft);
    mainLayout->addWidget(parent_buttons,0,3,1,2,Qt::AlignLeft);
    mainLayout->addWidget(presentation  ,1,0,1,2,Qt::AlignLeft);
    mainLayout->addWidget(edit          ,1,3,1,2,Qt::AlignLeft);
    mainLayout->addWidget(create        ,2,0,1,3,Qt::AlignLeft);
    mainLayout->addWidget(mark_delete   ,2,4,1,1,Qt::AlignLeft);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    create->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Expanding );

    connect(openAll, SIGNAL(clicked()), this, SLOT(openAllDLChildren()));
    connect(parent_buttons, SIGNAL(clicked()), this, SLOT(openParent()));
}

void FormObject::openExist(){
    presentation->setText(table_name);
    readFromDB();
    create->setText("Update");
    if (table_name==This_settings["EngWordTable"]){
        parent_buttons->hide();
        parent->hide();
        openAll->show();
    }
    connect(create, SIGNAL(clicked()), this, SLOT(UpdateToDB()));
    connect(mark_delete, SIGNAL(clicked()), this, SLOT(UpdateToDB()));
}

void FormObject::createNew(){
    presentation->setText("New "+table_name);
    mark_delete->hide();
    if (table_name==This_settings["EngWordTable"]){
        parent->hide();
        parent_buttons->hide();
    };
    connect(create, SIGNAL(clicked()), this, SLOT(addNewToDB()));
}

void FormObject::readFromDB(){
    QString transaction_name="read";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QString sql;
    if (table_name==This_settings["EngWordTable"]){
        sql="SELECT presentation from "+table_name+" where id=:id;";
    }else{
        sql="SELECT "
            "r.presentation, "
            "w.presentation,"
            "r.parent "
            "FROM "+table_name+" as r "
            "LEFT JOIN "+This_settings["EngWordTable"]+" as w ON "
            "r.parent = w.id "
            "WHERE r.id=:id";
    };
    query.prepare(sql);
    query.bindValue(":id",id.toInt());
    if(!query.exec()){
        GetErrorMessage(&db,transaction_name,&query);
        return;
    };
    query.next();
    QString Its_presentation=query.value(0).toString();
    edit->setText(Its_presentation);
    if (table_name!=This_settings["EngWordTable"]){
        QString Word_presentation=query.value(1).toString();
        parent_buttons->setText(Word_presentation);
        parent_id_to_open=query.value(2).toString();
    }

    //db.commit();
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
}

void FormObject::addNewToDB(){
    QString transaction_name="create";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QString sql;
    if (table_name==This_settings["EngWordTable"]){
        sql="INSERT INTO "+table_name+" (presentation) VALUES (:presentation);";
        query.prepare(sql);
        query.bindValue(":presentation",edit->text());
    }else{
        sql="INSERT INTO "+table_name+" (presentation,parent) VALUES (:presentation,:parent)";
        query.prepare(sql);
        query.bindValue(":parent", parent_id.toInt());
        query.bindValue(":presentation",edit->text());
    };
    if(!query.exec()){
        GetErrorMessage(&db,transaction_name,&query);
        return;
    };
    db.commit();
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
}

void FormObject::UpdateToDB(){
    int del_mark=1;
    if(this->sender()==mark_delete){
        del_mark=0;
    }
    QString _presentation=edit->text();

    QString transaction_name="update";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QString sql;
    sql="UPDATE "+table_name+" SET presentation=:presentation ,del_mark=:del_mark  WHERE id=:id;";
    query.prepare(sql);
    query.bindValue(":id",id.toInt());
    query.bindValue(":presentation",_presentation);
    query.bindValue(":del_mark",del_mark);
    if(!query.exec()){
        GetErrorMessage(&db,transaction_name,&query);
        return;
    };
    db.commit();
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
}

void FormObject::openAllDLChildren()
{
    if(id!=""){
        emit needOpenDL(id);
    };
}

void FormObject::openParent()
{
    //qDebug()<<parent_id_to_open;
    emit OpenObject(parent_id_to_open,This_settings["EngWordTable"]);
}





