#include "clients.h"
#include "ui_clients.h"
#include <QScreen>

Clients::Clients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clients)
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

    //Frames size:
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/1.45));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.8));


    //Set the table Size
    ui -> table_clients -> setColumnCount(9);
    ui->table_clients ->setColumnWidth(0,70);
    ui->table_clients ->setColumnWidth(1,static_cast<int>(width/14));  //Cliente
    ui->table_clients ->setColumnWidth(2,static_cast<int>(width/14));  //Regional
    ui->table_clients ->setColumnWidth(3,static_cast<int>(width/8));  //Nombre del contacto
    ui->table_clients ->setColumnWidth(4,static_cast<int>(width/12));  //Teléfono del contacto
    ui->table_clients ->setColumnWidth(5,static_cast<int>(width/12));  //E-mail del contacto
    ui->table_clients ->setColumnWidth(6,static_cast<int>(width/12));  //Cargo del contacto
    ui->table_clients ->setColumnWidth(7,static_cast<int>(width/8));  // Asesor de ventas
    ui->table_clients ->setColumnWidth(8,0);

}

Clients::~Clients()
{
    delete ui;
}
