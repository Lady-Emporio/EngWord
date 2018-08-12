#include "settings_gui.h"

Settings_gui::Settings_gui(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("Settings");
    All_Edit_Widgets.clear();
    makeGui();
    setDefaultValue();
}
void Settings_gui::makeGui(){
    QList<QString> keys=S::Settings()->All_QString_PARAMS.keys();

    QGridLayout  *mainLayout=new QGridLayout (this);
    int row;
    for(int i=0;i!=keys.length();++i){
        QString k=keys[i];
        QLabel *label=new QLabel(k,this);
        QLineEdit * edit=new QLineEdit(this);
        edit->setObjectName(k);
        mainLayout->addWidget(label,i,0,1,1,Qt::AlignLeft);
        mainLayout->addWidget(edit,i,1);
        label->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed );
        edit->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed );
        All_Edit_Widgets.insert(k,edit);
        row=i;
    };
    ++row;
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setLayout(mainLayout);
    QPushButton *update=new QPushButton("update",this);
    QPushButton *clear=new QPushButton("clear",this);
    mainLayout->addWidget(update,row,0,1,1,Qt::AlignHCenter|Qt::AlignTop);
    mainLayout->addWidget(clear,row,1,1,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(update, SIGNAL(clicked()), this, SLOT(update()));
    connect(clear, SIGNAL(clicked()), this, SLOT(setDefaultValue()));
}
void Settings_gui::update(){
    QList<QString> keys=S::Settings()->All_QString_PARAMS.keys();
    for(int i=0;i!=keys.length();++i){
        QString k=keys[i];
        S::Settings()->All_QString_PARAMS[k]=All_Edit_Widgets[k]->text();
    };
    S::Settings()->writeSettings();
}

void Settings_gui::setDefaultValue(){
    QList<QString> keys=S::Settings()->All_QString_PARAMS.keys();
    for(int i=0;i!=keys.length();++i){
        QString k=keys[i];
        QLineEdit * edit=All_Edit_Widgets[k];
        edit->setText(S::get(k));
    };
}
