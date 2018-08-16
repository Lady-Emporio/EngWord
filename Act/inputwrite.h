#ifndef INPUTWRITE_H
#define INPUTWRITE_H

#include <windows.h>
#include "./Settings/s.h"

class InputWrite : public QWidget
{
    Q_OBJECT
public:
    QMap<QString,QString> This_settings;
    QMap<int,QString> translateCods;
    QList <QMap<QString,QString>>WordsList;
    QMap<QString,QString> ActiveWords;
    void makeGui();
    void fillWordsList();
    void filltranslateCods();
    HHOOK hHook;

    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    QLabel *KeysState;
    QLabel *SoundState;
    QLabel *Eng_word;
    QLabel *editWord;
    QLabel *ruWord;
    QLabel *exampleWord;

    void getError(QString text);

    explicit InputWrite(QWidget *parent = 0);
    ~InputWrite();
    void startWithHook();
public slots:
    void getNextWord();
};

#endif // INPUTWRITE_H
