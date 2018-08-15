#ifndef FORMOBJECT_H
#define FORMOBJECT_H

#include "include.h"
#include "dynamiclist.h"
#include "Settings/s.h"
#include "separateclearfunc.h"
class FormObject : public QWidget
{
    Q_OBJECT
    QString table_name;
    QString id;
    QMap<QString,QString> This_settings;
    QString parent_id;
    QString parent_id_to_open;
    QString parent_table;
    void makeDefaultGui();
    void openExist();
    void createNew();
    void readFromDB();
    QPushButton *create;
    QPushButton *mark_delete;
    QPushButton *openAll;
    bool addTodb();
    bool getIdNotCreated(QString &id);
public:
    explicit FormObject(QWidget *parent = 0,QString table_name="",QString id="",QString parent_id="");
    //Таблица обязательна
    //id только для открытия
    //parent только для создания
    QLabel * presentation;
    QLineEdit *edit;
    QLabel * parent_label;
    QPushButton *parent_buttons;
public slots:
    void addNewToDB();
    void UpdateToDB();
    void openAllDLChildren();
    void openParent();
signals:
    void OpenObject(QString table,QString id);
    void needOpenDL(QString parent,QString thisIsTable);
    void OpenNowCreated(FormObject* its,QString table_name,QString id);
};

#endif // FORMOBJECT_H
