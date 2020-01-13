#include "page_1.h"
#include "ui_page_1.h"
#include <QScreen>

page_1::page_1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_1)
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

    //  Set icons
   double pix_w_b = (width*111)/1920;
   double pix_h_b= (height*60)/1080;

   QPixmap pix_client(":/images/images/cliente-fondo.png");
   ui->icon_cliente->setPixmap(pix_client.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_cliente->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

   //  Set icons
  double pix_w_a = (width*43)/1920;
  double pix_h_a = (height*43)/1080;

   QPixmap pix_search(":/images/images/lupa-fondo.png");
   QIcon ButtonIcon(pix_search.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_search->setIcon(ButtonIcon);
   ui->icon_search->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

   QPixmap pix_pluss(":/images/images/lapiz-fondo.png");
   QIcon ButtonIcon_2(pix_pluss.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_pluss->setIcon(ButtonIcon_2);
   ui->icon_pluss->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

}

page_1::~page_1()
{
    delete ui;
}

void page_1::on_pushButton_7_clicked()
{
    nights = new Nights(this);
    nights->show();
}

//This section is for "VÍA"
void page_1::on_mail_butt_clicked()
{

}

void page_1::on_visita_butt_clicked()
{

}

void page_1::on_telefono_butt_clicked()
{

}

void page_1::on_wpp_butt_clicked()
{

}

//This section is for "MOTIVO"
void page_1::on_tarifario_butt_clicked()
{

}

void page_1::on_cotizacion_butt_clicked()
{

}

void page_1::on_reserva_butt_clicked()
{

}

void page_1::on_otros_butt_clicked()
{

}
