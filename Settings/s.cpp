#include "s.h"
S::S()
{
    All_QString_PARAMS.insert("path/to/image/x.png", "E:/Qt project/EngWorld/Word/Internet Explorer_32.png");
    All_QString_PARAMS.insert("path/to/db.sqlite", "MainDB.sqlite");
    All_QString_PARAMS.insert("EngWordTable", "Word");
    All_QString_PARAMS.insert("EngTranslateTable", "Eng");
    All_QString_PARAMS.insert("RuTranslateTable", "Ru");
    All_QString_PARAMS.insert("exampleTable", "Example");
    All_QString_PARAMS.insert("TableToSelectWords", "SelectWord");
    All_QString_PARAMS.insert("limitSelect(int)", "20");
    All_QString_PARAMS.insert("Error/Click/Path/file.mp3", "E:/Qt project/TrySecondEngWord/NewEngWord/Sound/SoundEffect/VeryFastBoof.mp3");
    All_QString_PARAMS.insert("Wrong/Click/Path/file.mp3", "E:/Qt project/TrySecondEngWord/NewEngWord/Sound/SoundEffect/GoodNeedDoFastVeryLong.mp3");
    All_QString_PARAMS.insert("Alphabet/path/", "E:/Qt project/TrySecondEngWord/NewEngWord/Sound/alphabet/");
    All_QString_PARAMS.insert("dir/Word/", "E:/Qt project/TrySecondEngWord/NewEngWord/Sound/dirWord/");

    readSettings();
    CreateDBIfNotExist();
}
void S::CreateDBIfNotExist(){
    QString transaction_name="create";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(All_QString_PARAMS["path/to/db.sqlite"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QStringList sqls;
    sqls<<"CREATE TABLE IF NOT EXISTS "+All_QString_PARAMS["EngWordTable"]+" ("
          "id integer PRIMARY KEY autoincrement NOT NULL,"
          "presentation text UNIQUE NOT NULL,"
          "del_mark integer default 1 CHECK(del_mark =0 or del_mark =1) );"
        <<"CREATE TABLE IF NOT EXISTS "+All_QString_PARAMS["EngTranslateTable"]+" ( "
          "id integer PRIMARY KEY autoincrement NOT NULL,"
          "presentation text NOT NULL,"
          "parent integer REFERENCES word(id),"
          "del_mark integer default 1 CHECK(del_mark =0 or del_mark =1) );"
        <<"CREATE TABLE IF NOT EXISTS "+All_QString_PARAMS["RuTranslateTable"]+" ("
          "id integer PRIMARY KEY autoincrement NOT NULL,"
          "presentation text NOT NULL,"
          "parent integer REFERENCES word(id),"
          "del_mark integer default 1 CHECK(del_mark =0 or del_mark =1) );"
        <<"CREATE TABLE IF NOT EXISTS "+All_QString_PARAMS["exampleTable"]+" ( "
          "id integer PRIMARY KEY autoincrement NOT NULL,"
          "presentation text NOT NULL,"
          "parent integer REFERENCES word(id),"
          "del_mark integer default 1 CHECK(del_mark =0 or del_mark =1) );";
    for (int i=0;i!=sqls.length();++i){
        QString next_sql=sqls[i];
        if(!query.exec(next_sql)){
            GetErrorMessage(nullptr,transaction_name,&query);
            continue;
        };
        query.finish();
    };
    db.commit();
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
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
