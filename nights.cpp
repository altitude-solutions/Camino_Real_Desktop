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
    if(nights_number!=""){
        emit send_nights(nights_number);
        ui -> label_5 -> setText("");
        this->close();
    }
    else{
         QMessageBox::critical (this, "Error", "Ingresar el número de noches porfavor");
    }
}
