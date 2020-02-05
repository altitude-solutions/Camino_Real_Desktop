#include "reservas.h"
#include "ui_reservas.h"
#include <QScreen>

Reservas::Reservas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reservas)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width)/1.8;
    double size_h = (height)/1.6;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);
}

Reservas::~Reservas(){
    delete ui;
}

void Reservas::receiver(QString userName, QString realName, QString token, QString url){
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;
}
