#ifndef SELECTWORD_H
#define SELECTWORD_H

#include "include.h"
#include "Settings/s.h"
class SelectWord : public QWidget
{
    Q_OBJECT
    void makeGui();
    bool readFromDB();
    QMap<QString,QString> This_settings;

public:
    QListWidget *allWords;
    QListWidget *selectWord;
    explicit SelectWord(QWidget *parent = 0);

signals:
public slots:
    void selectItemAllWord(QListWidgetItem *item);
    void selectItemSelectedWord(QListWidgetItem *item);
    void addSelectWordToDB();
};

#endif // SELECTWORD_H
