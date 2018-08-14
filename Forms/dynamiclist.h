#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "include.h"
#include "./Settings/s.h"
class DynamicList : public QWidget
{
    Q_OBJECT
    QString table_name;
    QString mark_del;
    QString This_is_parents;
    void makeGui();
    QMap<QString,QString> This_settings;
    QListWidget *dynamic_list;
public:
    explicit DynamicList(QWidget *parent = 0, QString table_name="", QString mark_del="1",QString This_is_parents="");
public slots:
    void readFromDB();
    void open();
    void create();
signals:
    void OpenObject(QString id,QString table);
    void CreateObject(QString table,QString parent_id="");
};

#endif // DYNAMICLIST_H
