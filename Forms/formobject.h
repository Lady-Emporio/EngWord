#ifndef FORMOBJECT_H
#define FORMOBJECT_H

#include "include.h"

class FormObject : public QWidget
{
    Q_OBJECT
    QString table_name;
    QString id;
    QMap<QString,QString> This_settings;
    QString parent_id;
    QString parent_id_to_open;
    void makeDefaultGui();
    void openExist();
    void createNew();
    void readFromDB();
    QPushButton *create;
    QPushButton *mark_delete;
    QPushButton *openAll;
public:
    explicit FormObject(QWidget *parent = 0,QString table_name="",QString id="",QString parent_id="");
    //Таблица обязательна
    //id только для открытия
    //parent только для создания
    QLabel * presentation;
    QLineEdit *edit;
    QLabel * parent;
    QPushButton *parent_buttons;
public slots:
    void addNewToDB();
    void UpdateToDB();
    void openAllDLChildren();
    void openParent();
signals:
    void OpenObject(QString table,QString id);
    void needOpenDL(QString parent);
};

#endif // FORMOBJECT_H
