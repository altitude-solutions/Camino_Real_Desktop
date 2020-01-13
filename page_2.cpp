#include "page_2.h"
#include "ui_page_2.h"
#include <QScreen>

page_2::page_2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_2)
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
  double pix_w_a = (width*40)/1920;
  double pix_h_a = (height*40)/1080;

   QPixmap pix_search(":/images/images/lupa-fondo.png");
   QIcon ButtonIcon(pix_search.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_search->setIcon(ButtonIcon);
   ui->icon_search->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

   QPixmap pix_pluss(":/images/images/mas-fondo.png");
   QIcon ButtonIcon_2(pix_pluss.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_pluss->setIcon(ButtonIcon_2);
   ui->icon_pluss->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

   //Set pen icons
  double pix_w_c = (width*33)/1920;
  double pix_h_c = (height*33)/1080;

  QPixmap pen_pix(":/images/images/lapiz-fondo.png");
  QIcon pen_button(pen_pix.scaled( static_cast<int>(pix_w_c),static_cast<int>(pix_h_c), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  ui->icon_pen_1->setIcon(pen_button);
  ui->icon_pen_1->setIconSize(QSize(static_cast<int>(pix_w_c),static_cast<int>(pix_h_c)));
  ui->icon_pen_2->setIcon(pen_button);
  ui->icon_pen_2->setIconSize(QSize(static_cast<int>(pix_w_c),static_cast<int>(pix_h_c)));
  ui->icon_pen_3->setIcon(pen_button);
  ui->icon_pen_3->setIconSize(QSize(static_cast<int>(pix_w_c),static_cast<int>(pix_h_c)));
  ui->icon_pen_4->setIcon(pen_button);
  ui->icon_pen_4->setIconSize(QSize(static_cast<int>(pix_w_c),static_cast<int>(pix_h_c)));
}

page_2::~page_2()
{
    delete ui;
}
