#include "inputwrite.h"

LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    InputWrite* thisIW= (InputWrite*)S::Settings()->InpWr;
    static bool KillPress=true;
    static bool playing=true;

    if (wParam == WM_KEYDOWN ){
        KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);
        int key=cKey.vkCode;

        if (key==27){//27,QString("Esc")
            if(KillPress){
                qDebug()<<QString("send ahead");
                thisIW->KeysState->setText("Keys go next(\"Esc\")");
                KillPress=false;
            }else{
                qDebug()<<QString("break send");
                thisIW->KeysState->setText("Keys delete(\"Esc\")");
                KillPress=true;
            }
            return 1;
        };

        if (key==112){//112,QString("F1")
            if(playing){
                qDebug()<<QString("sound end");
                playing=false;
                thisIW->SoundState->setText("Sound close(\"F1\")");
            }else{
                qDebug()<<QString("sound start");
                playing=true;
                thisIW->SoundState->setText("Sound open(\"F1\")");
            }
            return 1;
        };

        if(!KillPress){
            return CallNextHookEx(thisIW->hHook, nCode, wParam, lParam);
        }

        if(thisIW->translateCods.contains(key)){
            if(playing){
                if(thisIW->translateCods.value(key)=="1"){
                    return 1;
                }else if(thisIW->translateCods.value(key)=="2"){

                    return 1;
                }else if(thisIW->translateCods.value(key)=="3"){
                    //thisIW->player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(S::get("dir/Word/")+thisIW->ActiveWords["Word_id"]+".mp3")));
                    //thisIW->player->play();
                    return 1;
                }
            }

            static int pos=0;
            QString value=thisIW->translateCods.value(key);
            QString needEnter=QString(thisIW->ActiveWords["Word"][pos]);
            if(playing){
                QRegExp rx("[a-z]{1}");    // соответствует целым от 0 до 99
                if(rx.indexIn(value)==-1 || value.length()>1){
                    thisIW->player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(S::get("Wrong/Click/Path/file.mp3"))));
                    thisIW->player->play();
                    return 1;
                }
            }
            if(value==needEnter){
                if(playing){
                    thisIW->player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(S::get("Alphabet/path/")+value+".mp3")));
                    thisIW->player->play();
                }
                ++pos;
                thisIW->editWord->setText( thisIW->editWord->text()+ needEnter);
                if(pos==thisIW->ActiveWords["Word"].length()){
                    pos=0;
                    thisIW->getNextWord();
                }
            }else{
                if(playing){
                    thisIW->playlist->clear();
                    thisIW->playlist->addMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(S::get("Error/Click/Path/file.mp3"))));
                    thisIW->playlist->addMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(S::get("Alphabet/path/")+value+".mp3")));
                    thisIW->playlist->setCurrentIndex(0);
                    thisIW->player->setPlaylist(thisIW->playlist);
                    thisIW->player->play();
                }
            }
        }else{
            if(playing){
                thisIW->player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(S::get("Wrong/Click/Path/file.mp3"))));
                thisIW->player->play();
            }
            return 1;
        }
        return 1;
    }else{
        return 1;
    }
    return CallNextHookEx(thisIW->hHook, nCode, wParam, lParam);

}

void InputWrite::startWithHook()
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
        if (hHook == NULL)
    {
        Eng_word->setText("Hook Failed");
        editWord->setText("Hook Failed");
        ruWord->setText("Hook Failed");
        exampleWord->setText("Hook Failed");
    }
}

InputWrite::InputWrite(QWidget *parent) : QWidget(parent)
{
    filltranslateCods();
    This_settings=S::Settings()->All_QString_PARAMS;
    hHook=NULL;
    makeGui();
    fillWordsList();
    getNextWord();
}

InputWrite::~InputWrite()
{
    if(hHook!=NULL){
        if(UnhookWindowsHookEx(hHook)==0){
            getError("Не получилось UnhookWindowsHookEx.");
        };
    }
}

void InputWrite::getNextWord()
{
    Eng_word->clear();
    editWord->clear();
    ruWord->clear();
    exampleWord->clear();

    if(WordsList.empty()){
        getError("Empty Word list");
        return;
    }
    ActiveWords=WordsList.takeLast();
    Eng_word->setText(ActiveWords["Eng"]);
    //ruWord->setText(ActiveWords["Ru"]);
    //exampleWord->setText(ActiveWords["Example"]);
}

void InputWrite::makeGui()
{
    player=new QMediaPlayer(this);
    playlist=new QMediaPlaylist(this);
    QVBoxLayout  *mainLayout=new QVBoxLayout (this);
    Eng_word=new QLabel(this);
    editWord=new QLabel(this);
    ruWord=new QLabel(this);
    exampleWord=new QLabel(this);
    KeysState=new QLabel("Keys delete(\"Esc\")",this);
    SoundState=new QLabel("Sound open(\"F1\")",this);
    QHBoxLayout *row1=new QHBoxLayout(this);
    row1->addWidget(KeysState);
    row1->addWidget(SoundState);
    mainLayout->addWidget(Eng_word);
    mainLayout->addWidget(editWord);
    mainLayout->addWidget(exampleWord);
    mainLayout->addWidget(ruWord);
    mainLayout->addLayout(row1);
}

void InputWrite::fillWordsList()
{
    WordsList.clear();

    QString transaction_name="fillWordsList";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",transaction_name);
    db.setDatabaseName(This_settings["path/to/db.sqlite"]);//Имя базы.
    if (!db.open()){
        GetErrorMessage(&db,transaction_name);
        return;
    };
    QSqlQuery query(db);
    QString sql;
    sql="Select Word_id,Word,Eng,Example,Ru FROM "+This_settings["TableToSelectWords"]+" ORDER BY Word_id LIMIT "+This_settings["limitSelect(int)"]+";";

    if(!query.exec(sql)){
        GetErrorMessage(&db,transaction_name,&query);
        return;
    };
    while(query.next()){
        QMap<QString,QString> nextRow;
        nextRow.insert("Eng",query.value("Eng").toString());
        nextRow.insert("Word",query.value("Word").toString());
        nextRow.insert("Ru",query.value("Ru").toString());
        nextRow.insert("Example",query.value("Example").toString());
        nextRow.insert("Word_id",query.value("Word_id").toString());
        WordsList.append(nextRow);
    }
    db.close();
    db=QSqlDatabase();
    QSqlDatabase::removeDatabase(transaction_name);
}

void InputWrite::filltranslateCods()
{
    translateCods.insert(112,QString("F1"));
    translateCods.insert(113,QString("F2"));
    translateCods.insert(114,QString("F3"));
    translateCods.insert(115,QString("F4"));
    translateCods.insert(116,QString("F5"));
    translateCods.insert(117,QString("F6"));
    translateCods.insert(118,QString("F7"));
    translateCods.insert(119,QString("F8"));
    translateCods.insert(120,QString("F9"));
    translateCods.insert(121,QString("F10"));
    translateCods.insert(32,QString("пробел"));
    translateCods.insert(8,QString("BackSpace"));
    translateCods.insert(9,QString("Tab"));
    translateCods.insert(13,QString("Enter"));
    translateCods.insert(16,QString("Shift"));
    translateCods.insert(17,QString("Ctrl"));
    translateCods.insert(18,QString("Alt"));
    translateCods.insert(20,QString("CapsLock"));
    translateCods.insert(27,QString("Esc"));
    translateCods.insert(45,QString("Insert"));
    translateCods.insert(33,QString("PageUp"));
    translateCods.insert(34,QString("PageDown"));
    translateCods.insert(35,QString("End"));
    translateCods.insert(36,QString("Home"));
    translateCods.insert(37,QString("курсор ←"));
    translateCods.insert(38,QString("курсор ↑"));
    translateCods.insert(39,QString("курсор →"));
    translateCods.insert(40,QString("курсор ↓"));
    translateCods.insert(46,QString("Delete"));
    translateCods.insert(44,QString("PrintScreen"));
    translateCods.insert(145,QString("ScrollLock"));
    translateCods.insert(48,QString("0"));
    translateCods.insert(49,QString("1"));
    translateCods.insert(50,QString("2"));
    translateCods.insert(51,QString("3"));
    translateCods.insert(52,QString("4"));
    translateCods.insert(53,QString("5"));
    translateCods.insert(54,QString("6"));
    translateCods.insert(55,QString("7"));
    translateCods.insert(56,QString("8"));
    translateCods.insert(57,QString("9"));
    translateCods.insert(192,QString("`"));
    translateCods.insert(189,QString("-"));
    translateCods.insert(187,QString("="));
    translateCods.insert(65,QString("a"));
    translateCods.insert(66,QString("b"));
    translateCods.insert(67,QString("c"));
    translateCods.insert(68,QString("d"));
    translateCods.insert(69,QString("e"));
    translateCods.insert(70,QString("f"));
    translateCods.insert(71,QString("g"));
    translateCods.insert(72,QString("h"));
    translateCods.insert(73,QString("i"));
    translateCods.insert(74,QString("j"));
    translateCods.insert(75,QString("k"));
    translateCods.insert(76,QString("l"));
    translateCods.insert(77,QString("m"));
    translateCods.insert(78,QString("n"));
    translateCods.insert(79,QString("o"));
    translateCods.insert(80,QString("p"));
    translateCods.insert(81,QString("q"));
    translateCods.insert(82,QString("r"));
    translateCods.insert(83,QString("s"));
    translateCods.insert(84,QString("t"));
    translateCods.insert(85,QString("u"));
    translateCods.insert(86,QString("v"));
    translateCods.insert(87,QString("w"));
    translateCods.insert(88,QString("x"));
    translateCods.insert(89,QString("y"));
    translateCods.insert(90,QString("z"));
}


void InputWrite::getError(QString text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}


