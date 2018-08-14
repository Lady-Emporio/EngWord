#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include.h"
#include "Settings/s.h"
#include "Settings/settings_gui.h"

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
    void needOpenDL(QString parent);
};

#endif // MAINWINDOW_H
