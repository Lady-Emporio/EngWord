#ifndef SETTINGS_GUI_H
#define SETTINGS_GUI_H

#include "include.h"
#include "Settings/s.h"
class Settings_gui : public QWidget
{
    Q_OBJECT
    void makeGui();

    QMap<QString,QLineEdit*> All_Edit_Widgets;
public:
    explicit Settings_gui(QWidget *parent = 0);

signals:

public slots:
    void update();
    void setDefaultValue();
};

#endif // SETTINGS_GUI_H
