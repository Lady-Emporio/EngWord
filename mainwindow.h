#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "include.h"
#include "Settings/settings_gui.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QMdiArea * mdiArea;
private://group function
    void createAndFillmdiArea();
private slots:
    void action_open_Settings();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
