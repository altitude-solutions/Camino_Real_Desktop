#include "inout.h"
#include "ui_inout.h"
#include <QScreen>

InOut::InOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InOut)
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

InOut::~InOut()
{
    delete ui;
}

void InOut::receive_info(QString userName, QString realName, QString token, QString url){

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

    l_a = new QVBoxLayout(this);
    p1 = new page_1(this);

    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p1, SLOT(receiver(QString, QString, QString, QString)));
    connect(p1,SIGNAL(send_clients()),this, SLOT(receive_clients()));
    emit send_info(this->userName, this->realName, this->token, this->url);

    l_a-> addWidget(p1);
    l_a ->setMargin (0);

    ui->widget->setLayout (l_a);
    actual_widget = "page_1";

    ui -> pushButton -> setStyleSheet(released);
    ui -> pushButton_2 -> setStyleSheet(pushed);

}

void InOut::on_pushButton_clicked()
{
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

    if(actual_widget=="page_2"){
       l_a->removeItem(l_a->itemAt(0));
        delete p2;
       ui->pushButton->setStyleSheet(released);
       ui->pushButton_2->setStyleSheet(pushed);
        p1 = new page_1(this);
        connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p1, SLOT(receiver(QString, QString, QString, QString)));
        connect(p1,SIGNAL(send_clients()),this, SLOT(receive_clients()));
        emit send_info(this->userName, this->realName, this->token, this->url);
        l_a->addWidget (p1);
        actual_widget = "page_1";
    }
    else{
        ui->pushButton->setStyleSheet(released);
        ui->pushButton_2->setStyleSheet(pushed);
    }
}

void InOut::on_pushButton_2_clicked()
{
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

    if(actual_widget=="page_1"){
        l_a->removeItem(l_a->itemAt(0));
        delete p1;
        ui->pushButton->setStyleSheet(pushed);
        ui->pushButton_2->setStyleSheet(released);
        p2 = new page_2(this);
        connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p2, SLOT(receiver(QString, QString, QString, QString)));
        connect(p2,SIGNAL(send_clients()),this, SLOT(receive_clients()));
        emit send_info(this->userName, this->realName, this->token, this->url);

        l_a->addWidget (p2);
        actual_widget = "page_2";
    }
    else{
        ui->pushButton->setStyleSheet(pushed);
        ui->pushButton_2->setStyleSheet(released);
    }
}

void InOut::receive_clients(){
    emit send_clients();
}
