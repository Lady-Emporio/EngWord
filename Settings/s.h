#ifndef S_H
#define S_H
#include "include.h"
#include "separateclearfunc.h"
class S
{
    S();
public:
    void * MyGlobalApp;
    void * InpWr;
    QMap<QString,QString> All_QString_PARAMS;
    static S * Settings();
    static QString get(QString param);
    void readSettings();
    void writeSettings();
    void insertInFile(QString key,QString value);
    void CreateDBIfNotExist();
};

#endif // S_H
