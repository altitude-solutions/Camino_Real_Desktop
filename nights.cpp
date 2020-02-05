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

   double size_w = (width);
   double size_h = (height*0.9);
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this -> setMinimumSize(size);

    //Set frame sizes
    ui -> frame ->setFixedWidth(static_cast<int>(width*0.18));
    ui -> frame_3 ->setFixedWidth(static_cast<int>(width*0.18));
    ui -> frame_4 ->setFixedWidth(static_cast<int>(width*0.18));
    ui -> frame_5 ->setFixedWidth(static_cast<int>(width*0.18));
}

Nights::~Nights()
{
    delete ui;
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

     double w = (width);
     double h = (height*0.9);

    emit send_info_box(icon, header, text, w, h);
    box_info->show();

}

void Nights::on_pushButton_clicked(){
    emit send_nights("","","","");
    this->close();
}

void Nights::on_pushButton_9_clicked()
{
    QString nights_number = ui-> nights -> text();
    QString early = ui -> nights_2 -> text();
    QString late = ui -> nights_3 -> text();
    QString upgrade = ui -> nights_4 -> text();

    emit send_nights(nights_number, early, late, upgrade);
    this -> close();
}


void Nights::on_nights_editingFinished(){
    QString benefit =  ui -> nights -> text();

    if(benefit!=""){
        bool validator = false;
        double validation =  benefit.toDouble(&validator);

        if(validation <= 0){
            information_box("x","Error","Ingresar un número válido porfavor");
             ui -> nights -> setText("");
        }
    }
}

void Nights::on_nights_2_editingFinished(){

    QString benefit =  ui -> nights_2 -> text();

    if(benefit!=""){
        bool validator = false;
        double validation =  benefit.toDouble(&validator);

        if(validation <= 0){
            information_box("x","Error","Ingresar un número válido porfavor");
             ui -> nights_2 -> setText("");
        }
    }
}

void Nights::on_nights_3_editingFinished(){
    QString benefit =  ui -> nights_3 -> text();

    if(benefit!=""){
        bool validator = false;
        double validation =  benefit.toDouble(&validator);

        if(validation <= 0){
            information_box("x","Error","Ingresar un número válido porfavor");
             ui -> nights_3 -> setText("");
        }
    }
}


void Nights::on_nights_4_editingFinished(){
    QString benefit =  ui -> nights_4 -> text();

    if(benefit!=""){
        bool validator = false;
        double validation =  benefit.toDouble(&validator);

        if(validation <= 0){
            information_box("x","Error","Ingresar un número válido porfavor");
             ui -> nights_4 -> setText("");
        }
    }
}
