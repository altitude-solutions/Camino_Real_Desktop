#include "records.h"
#include "ui_records.h"
#include <QScreen>

Records::Records(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Records)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width);
    double size_h = (height*0.9);
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);
}

Records::~Records(){
    delete ui;
}

void Records::receive_info(QString userName, QString realName, QString token, QString url){

    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    QString pushed = "font: 12pt \"MS Shell Dlg 2\";"
            "color:white;"
            "background-color:rgba(153,153,153,50%);"
            "min-width:7em;"
            "max-width:7em;"
            "min-height:2em;"
            "max-height:2em;";

    QString released = "font: 12pt \"MS Shell Dlg 2\";"
                    "color:white;"
                    "background-color:#79634E;"
                    "min-width:7em;"
                    "max-width:7em;"
                    "min-height:2em;"
                    "max-height:2em;";

    actual_widget = "page_1";

    l_a = new QVBoxLayout(this);
    p1 = new page_1(this);

    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p1, SLOT(receiver(QString, QString, QString, QString)));
    connect(p1,SIGNAL(send_clients()),this, SLOT(receive_clients()));

    connect(p1,SIGNAL(send_nights_back()),this,SLOT(activate_nights()));
    connect (this, SIGNAL(send_nights_back(QString)),p1,SLOT(receive_nights(QString)));

    //connect information box
    connect(p1,SIGNAL(send_info_box(QString, QString, QString)),this,SLOT(information_box(QString,QString, QString)));

    emit send_info(this->userName, this->realName, this->token, this->url);
    l_a-> addWidget(p1);
    l_a ->setMargin (0);

    ui->widget->setLayout (l_a);

    ui -> pushButton -> setStyleSheet(released);
    ui -> pushButton_2 -> setStyleSheet(pushed);

}
