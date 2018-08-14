#ifndef SEPARATECLEARFUNC_H
#define SEPARATECLEARFUNC_H
#include "include.h"

inline void GetErrorMessage(QSqlDatabase *db=nullptr,QString transaction_name="",QSqlQuery *query=nullptr){
    QMessageBox msgBox;
    QString error="";
    if(db!=nullptr){
       error+=db->lastError().text();
    }
    if(query!=nullptr){
        error+=" | "+query->lastError().text();
        query->finish();
    }
    msgBox.setText(error);
    msgBox.exec();
    if(db!=nullptr){
       db->close();
       db=nullptr;
    }
    QSqlDatabase::removeDatabase(transaction_name);
}

#endif // SEPARATECLEARFUNC_H
