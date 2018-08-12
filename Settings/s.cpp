#include "s.h"

S::S()
{
    All_QString_PARAMS.insert("path/to/image", "E:/Qt project/EngWorld/Word/Internet Explorer_32.png");
    All_QString_PARAMS.insert("path/to/db", "MainDB.sqlite");
    All_QString_PARAMS.insert("EngWordTable", "Word");
    All_QString_PARAMS.insert("EngTranslateTable", "Eng");
    All_QString_PARAMS.insert("RuTranslateTable", "Ru");
    All_QString_PARAMS.insert("exampleTable", "Example");
    readSettings();
}
S * S::Settings(){
    static S *setting=new S();
    return setting;
}

QString S::get(QString param){
    if (S::Settings()->All_QString_PARAMS.contains(param)){
        return Settings()->All_QString_PARAMS[param];
    }else{
        QMessageBox msgBox;
        msgBox.setText("In Settings not: "+param);
        msgBox.exec();
        return "";
    }
}

void S::readSettings(){
    QFile file("style.json");
    if(!file.exists()){
        writeSettings();
        return;
    }
    file.open(QIODevice::ReadOnly);
    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonObject root=document.object();

    QMapIterator<QString, QString> i(All_QString_PARAMS);
    while (i.hasNext()) {
        i.next();
        if(!root.contains(i.key())){
            insertInFile(i.key(),i.value());
        }else{
            All_QString_PARAMS[i.key()]=root[i.key()].toString();
        }
    };
}

void S::writeSettings(){
    qDebug()<<"Create json and write";
    QMapIterator<QString, QString> i(All_QString_PARAMS);
    QJsonObject root;
    while (i.hasNext()) {
        i.next();
        QJsonValue JSvalue(i.value());
        root.insert(i.key(), JSvalue);
    };
    QJsonDocument new_doc(root);
    QFile jsonFile("style.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(new_doc.toJson());
    jsonFile.close();
}

void S::insertInFile(QString key,QString value){
    qDebug()<<"insert in file: "<<key<<" | "<<value;
    QFile file("style.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject root=document.object();
    QJsonValue JSvalue(value);
    root.insert(key, JSvalue);

    QJsonDocument new_doc(root);
    QFile jsonFile("style.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(new_doc.toJson());
    jsonFile.close();
}
