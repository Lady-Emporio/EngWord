#ifndef MDI_DL_H
#define MDI_DL_H

#include "include.h"
#include "./Forms/dynamiclist.h"

class MDi_DL : public QWidget
{
    Q_OBJECT
    QMdiArea *mdiArea;
    QString parent_id;
    QMap<QString,QString> This_settings;
    void MakeGui();
    void AddDlLists();
public:
    explicit MDi_DL(QWidget *parent = 0,QString parent_id="");
signals:

public slots:
};

#endif // MDI_DL_H
