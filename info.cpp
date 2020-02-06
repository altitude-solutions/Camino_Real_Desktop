#include "info.h"
#include "ui_info.h"
#include <QScreen>
#include <QDebug>

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

   //Set Widget not resizable
   this->setMinimumWidth(width);
   this->setMinimumHeight(height);

}

Info::~Info(){
    delete ui;
}

void Info::receive_text(QString text){
    ui -> label_description -> setText(text);
}

void Info::mousePressEvent(QMouseEvent *event){
    qDebug()<<event;
    this ->close();
}

