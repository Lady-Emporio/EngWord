#include "selectword.h"

SelectWord::SelectWord(QWidget *parent) : QWidget(parent)
{
    This_settings=S::Settings()->All_QString_PARAMS;
    makeGui();
    if(!readFromDB()){
        return;
    }
}

void SelectWord::selectItemAllWord(QListWidgetItem *item)
{
    allWords->removeItemWidget(item);
    QListWidgetItem *newItem=new QListWidgetItem(item->text(),selectWord);
    newItem->setWhatsThis(item->whatsThis());
    delete item;
    selectWord->addItem(newItem);
}

void SelectWord::selectItemSelectedWord(QListWidgetItem *item)
{
    selectWord->removeItemWidget(item);
    QListWidgetItem *newItem=new QListWidgetItem(item->text(),allWords);
    newItem->setWhatsThis(item->whatsThis());
    delete item;
    allWords->addItem(newItem);
}

void SelectWord::addSelectWordToDB()
{
    QList<QStringList> listwidget;
    for(int i=0;i!=selectWord->count();++i){
        QListWidgetItem * oldItem=selectWord->item(i);
        QStringList sublist;
        sublist.append(oldItem->text());
        sublist.append(oldItem->whatsThis());
        listwidget.append(sublist);
    };

    QString transaction_name="addSelectWord";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db.sqlite"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QString sql;
    sql="CREATE TABLE IF NOT EXISTS "+This_settings["TableToSelectWords"]+" ( "
    " id integer PRIMARY KEY autoincrement NOT NULL, "
    " Word text, "
    " Eng text , "
    " Example text , "
    " Ru text , "
    " Word_id text , "
    " Eng_id text , "
    " Example_id text , "
    " Ru_id text );";

    //    "Word|Eng|Example|Ru"
    if(!query.exec(sql)){
        GetErrorMessage(&db,transaction_name,&query);
        return;
    };
    query.finish();
    for(int i=0;i!=listwidget.count();++i){
        QStringList next=listwidget[i];
        QStringList ids=next[1].split("|");
        QStringList Words=next[0].split("|");
        sql="Insert Into "+This_settings["TableToSelectWords"]+"(Word,Eng,Example,Ru,Word_id,Eng_id,Example_id,Ru_id) Values"
                "(:Word,:Eng,:Example,:Ru,:Word_id,:Eng_id,:Example_id,:Ru_id);";
        query.prepare(sql);
        query.bindValue(":Word",Words[0]);
        query.bindValue(":Eng",Words[1]);
        query.bindValue(":Example",Words[2]);
        query.bindValue(":Ru",Words[3]);
        query.bindValue(":Word_id",ids[0]);
        query.bindValue(":Eng_id",ids[1]);
        query.bindValue(":Example_id",ids[2]);
        query.bindValue(":Ru_id",ids[3]);
        if(!query.exec()){
            GetErrorMessage(&db,transaction_name,&query);
            return;
        };
        query.finish();
    }
    db.commit();
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);

}

bool SelectWord::readFromDB()
{
    allWords->clear();
    selectWord->clear();

    QString transaction_name="read select table";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db.sqlite"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return false;
    };
    QSqlQuery query(db);
    QString sql;
    sql=    " select "
            " Word.id as W_id, "
            " Word.presentation as W_p, "
            " Eng.id as Eng_id, "
            " Eng.presentation as Eng_p, "
            " Example.id as Example_id, "
            " Example.presentation Example_p, "
            " Ru.id as Ru_id, "
            " Ru.presentation as Ru_p "
            " from "+This_settings["EngWordTable"]+" as Word "
            " left join "+This_settings["EngTranslateTable"] +" as Eng on "
            " Eng.parent=Word.id"
            " left join "+ This_settings["exampleTable"]+ " as Example on "
            " Eng.id=Example.parent "
            " left join " +This_settings["RuTranslateTable"]+" as Ru on "
            " Eng.id=Ru.parent where Eng.presentation IS NOT NULL ; ";
    if(!query.exec(sql)){
        GetErrorMessage(nullptr,transaction_name,&query);
        return false;
    };
    while(query.next()){
        QString Word_id=query.value("W_id").toString();
        QString Eng_id=query.value("Eng_id").toString();
        QString Example_id=query.value("Example_id").toString();
        QString Ru_id=query.value("Ru_id").toString();

        QString Word_p=query.value("W_p").toString();
        QString Eng_p=query.value("Eng_p").toString();
        QString Example_p=query.value("Example_p").toString();
        QString Ru_p=query.value("Ru_p").toString();

        QString joinTitle=Word_p+"|"+Eng_p+"|"+Example_p+"|"+Ru_p;
        QString joinId=Word_id+"|"+Eng_id+"|"+Example_id+"|"+Ru_id;
        QListWidgetItem * nextWitem=new QListWidgetItem(joinTitle,allWords);
        nextWitem->setWhatsThis(joinId);
        allWords->addItem(nextWitem);
    }
    query.finish();
    db.commit();
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
    return true;
}

void SelectWord::makeGui()
{
    allWords=new QListWidget(this);
    selectWord=new QListWidget(this);
    QVBoxLayout  *mainLayout=new QVBoxLayout (this);
    this->setLayout(mainLayout);
    QHBoxLayout* mainHorLayout=new QHBoxLayout(this);
    QVBoxLayout  *leftLayout=new QVBoxLayout (this);
    QVBoxLayout  *rightLayout=new QVBoxLayout (this);
    QLabel * labelTitle=new QLabel("Word|Eng|Example|Ru",this);
    QPushButton * sendWordsButton=new QPushButton("Select",this);
    labelTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labelTitle);
    mainLayout->addLayout(mainHorLayout);
    mainLayout->addWidget(sendWordsButton);
    mainHorLayout->addLayout(leftLayout);
    mainHorLayout->addLayout(rightLayout);
    leftLayout->addWidget(allWords);
    rightLayout->addWidget(selectWord);

    connect(allWords, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(selectItemAllWord(QListWidgetItem *)));
    connect(selectWord, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(selectItemSelectedWord(QListWidgetItem *)));
    connect(sendWordsButton, SIGNAL(clicked()), this, SLOT(addSelectWordToDB()));
}
