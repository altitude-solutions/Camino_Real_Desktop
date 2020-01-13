#include "login.h"
#include "ui_login.h"
#include <QScreen>
#include <QPixmap>
#include <QDesktopWidget>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    main_window.hide();

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //frames sized
   ui -> frame_1 -> setFixedWidth(static_cast<int>(width*0.15));

   //set widget size dynamic, aspect ratio 16:9
   double size_w = (width)/3;
   double size_h = (height)/3;
   QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
   this->setFixedSize(size);

   //  Set icons
  double pix_w_a = (width*220)/1920;
  double pix_h_a = (height*220)/1080;

  QPixmap main_pix(":/images/images/camino_real.png");
  ui->main_icon->setPixmap(main_pix.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_w_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->main_icon->setFixedSize(static_cast<int>(pix_w_a), static_cast<int>(pix_h_a));

  //Connects
  connect(&main_window, &MainWindow::logOut,this,&Login::closeSession);

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    main_window.show();
    this->hide();
}

void Login::closeSession()
{
    main_window.close();
    this->show();
}
