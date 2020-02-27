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

    actual_widget = "page_2";

    l_a = new QVBoxLayout(this);
    tasks = new Tasks(this);

    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),tasks, SLOT(receiver(QString, QString, QString, QString)));

    //connect information box
    connect(tasks,SIGNAL(send_info_box(QString, QString, QString)),this,SLOT(information_box(QString,QString, QString)));

    emit send_info(this->userName, this->realName, this->token, this->url);
    l_a-> addWidget(tasks);
    l_a ->setMargin (0);

    ui->widget->setLayout (l_a);

    ui->page_1->setStyleSheet(pushed);
    ui->page_2->setStyleSheet(released);
    ui->page_3->setStyleSheet(pushed);

}

void Records::on_page_1_clicked(){

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
        delete tasks;
    }
    else if(actual_widget=="page_3"){
        l_a->removeItem(l_a->itemAt(0));
        delete reservas;
    }
    else{
        l_a->removeItem(l_a->itemAt(0));
        delete notask;
    }

    actual_widget = "page_1";
    notask = new No_task(this);
    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),notask, SLOT(receiver(QString, QString, QString, QString)));

    //connect information box
    connect(notask,SIGNAL(send_info_box(QString, QString, QString)),this,SLOT(information_box(QString,QString, QString)));

    emit send_info(this->userName, this->realName, this->token, this->url);
    l_a->addWidget (notask);

    ui->page_1->setStyleSheet(released);
    ui->page_2->setStyleSheet(pushed);
    ui->page_3->setStyleSheet(pushed);

}

void Records::on_page_2_clicked(){

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
        delete notask;
    }
    else if(actual_widget=="page_3"){
        l_a->removeItem(l_a->itemAt(0));
        delete reservas;
    }
    else{
        l_a->removeItem(l_a->itemAt(0));
        delete tasks;
    }

    actual_widget = "page_2";
    tasks = new Tasks(this);
    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),tasks, SLOT(receiver(QString, QString, QString, QString)));

    //connect information box
    connect(tasks,SIGNAL(send_info_box(QString, QString, QString)),this,SLOT(information_box(QString,QString, QString)));

    emit send_info(this->userName, this->realName, this->token, this->url);
    l_a->addWidget (tasks);

    ui->page_1->setStyleSheet(pushed);
    ui->page_2->setStyleSheet(released);
    ui->page_3->setStyleSheet(pushed);
}

void Records::on_page_3_clicked(){

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
        delete notask;
    }
    else if(actual_widget=="page_2"){
        l_a->removeItem(l_a->itemAt(0));
        delete tasks;
    }
    else{
        l_a->removeItem(l_a->itemAt(0));
        delete reservas;
    }

    actual_widget = "page_3";
    reservas = new Reservas(this);
    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),reservas, SLOT(receiver(QString, QString, QString, QString)));

    //connect information box
    connect(reservas,SIGNAL(send_info_box(QString, QString, QString)),this,SLOT(information_box(QString,QString, QString)));

    //Connect with Benefits upgrade
    connect(reservas,SIGNAL(send_benefits_id(QString, QString, QString, QString, QString, QString)),this,SLOT(benefits_update(QString, QString, QString, QString, QString, QString)));

    emit send_info(this->userName, this->realName, this->token, this->url);
    l_a->addWidget (reservas);

    ui->page_1->setStyleSheet(pushed);
    ui->page_2->setStyleSheet(pushed);
    ui->page_3->setStyleSheet(released);
}

void Records::benefits_update(QString register_id, QString early, QString late, QString upgrade, QString noshow, QString nights){

    benefits = new Update_benefits(this);
    connect(this, SIGNAL(send_info_id(QString, QString, QString, QString, QString, QString, QString, QString, QString)),benefits,SLOT(receive_info(QString, QString, QString,QString, QString, QString, QString, QString, QString)));

    //connect information box
    connect(benefits,SIGNAL(send_info_box(QString, QString, QString)),this,SLOT(information_box(QString,QString, QString)));

    //Table update
    connect(benefits,SIGNAL(send_update()),reservas,SLOT(updater()));

    emit send_info_id(this -> userName, this -> token, this -> url, register_id, early, late, upgrade, noshow, nights);
    benefits->show();
}

void Records::information_box(QString icon, QString header, QString text){

    box_info = new Information_box(this);
    connect(this, SIGNAL(send_info_box(QString, QString, QString, double, double)),box_info, SLOT(receive_info(QString,QString, QString, double, double)));

    //Get screen Size
   const auto screens = qApp->screens();

    int width = screens[0]->geometry().width();
    int height = screens[0]->geometry().height();

     //set widget size dynamic, aspect ratio 16:9
    double w = (width);
    double h = (height*0.9);

    emit send_info_box(icon, header, text, w, h);
    box_info->show();

}
