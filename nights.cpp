#include "nights.h"
#include "ui_nights.h"
#include <QScreen>
#include <QMessageBox>

Nights::Nights(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Nights)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width)/2;
    double size_h = (height)/1.8;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

}

Nights::~Nights()
{
    delete ui;
}

void Nights::on_pushButton_9_clicked()
{
    QString nights_number = ui-> nights -> text();

    bool validator = false;
    double validation =  nights_number.toDouble(&validator);

    if(validation > 0){
        if(nights_number!=""){
            emit send_nights(nights_number);
            ui -> nights -> setText("");
            this->close();
        }
        else{
            information_box("x","Error","Ingresar el número de noches porfavor");
             //QMessageBox::critical (this, "Error", "Ingresar el número de noches porfavor");
             ui -> nights -> setText("");
        }
    }
    else{
        information_box("x","Error","Ingresar un número válido porfavor");
        //QMessageBox::critical (this, "Error", "Ingresar un número válido porfavor");
        ui -> nights -> setText("");
    }
}

void Nights::receive_page(QString page){
    this ->actual_page = page;
}

void Nights::information_box(QString icon, QString header, QString text){

    box_info = new Information_box(this);
    connect(this, SIGNAL(send_info_box(QString, QString, QString, double, double)),box_info, SLOT(receive_info(QString,QString, QString, double, double)));

    //Get screen Size
   const auto screens = qApp->screens();

    int width = screens[0]->geometry().width();
    int height = screens[0]->geometry().height();

     //set widget size dynamic, aspect ratio 16:9
     double w = (width)/2;
     double h = (height)/1.8;

    emit send_info_box(icon, header, text, w, h);
    box_info->show();

}

void Nights::on_pushButton_clicked()
{
    emit send_nights("");
    this->close();
}
