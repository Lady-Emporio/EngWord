#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include.h"
#include "Settings/s.h"
#include "Settings/settings_gui.h"
#include "./Forms/formobject.h"
#include "Settings/settings_gui.h"
#include "./Forms/dynamiclist.h"
#include "./Forms/mdi_dl.h"
#include "./Act/inputwrite.h"
#include "./Act/selectword.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMdiArea *mdiArea;
public slots:
    void OpenObject(QString id, QString table);
    void action_Settings();
    void action_Dynamic_EngTable();
    void CreateObject(QString table,QString parent_id="");
    void needOpenDL(QString parent,QString table_name);
    void OpenNowCreated(FormObject* needClose,QString table_name,QString id);
    void action_SelectWord();
    void action_InputWrite();
};

#endif // MAINWINDOW_H
