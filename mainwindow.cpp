#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

   //Set Widget not resizable
   this->setMinimumWidth(width);
   this->setMinimumHeight(height);

   //set widget size maximized
   this->setWindowState(Qt::WindowMaximized);

   //Set icon
   QIcon icon(":/images/images/icono.ico");
   this -> setWindowIcon(icon);

   //set Window title
   this->setWindowTitle("Clientes corporativos");

    //Set frames
    ui->frame->setFixedHeight(static_cast<int>(height*0.1));
    ui->frame_2->setFixedHeight(static_cast<int>(height*0.9));

   double pix_w = width;
   double pix_h = (height*0.9);

   QPixmap pixmap(":/images/images/dark_background.jpg");
   QPalette palette;

   palette.setBrush(QPalette::Background, pixmap.scaled(static_cast<int>(pix_w),static_cast<int>(pix_h)));
   ui->frame_2->setAutoFillBackground(true);
   ui->frame_2->setPalette(palette);

   // Set icons
  double pix_w_a = (width*100)/1920;
  double pix_h_a = (height*100)/1080;

  QPixmap main_pix(":/images/images/camino_real.png");
  ui->main_icon->setPixmap(main_pix.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_w_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->main_icon->setFixedSize(static_cast<int>(pix_w_a), static_cast<int>(pix_h_a));

   //set Icons
  double pix_w_b = (width*30)/1920;
  double pix_h_b = (height*30)/1080;
  QPixmap back_pix(":/images/images/atras.png");
  QIcon ButtonIcon(back_pix.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_back->setIcon(ButtonIcon);
  ui->icon_back->setIconSize(QSize(static_cast<int>(pix_w_b),static_cast<int>(pix_h_b)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {

        QString pushed = "font: 14pt \"MS Shell Dlg 2\";"
                "color:white;"
                "background-color:rgba(153,153,153,50%);"
                "min-width:7em;"
                "max-width:7em;"
                "min-height:2.5em;"
                "max-height:2.5em;";

        QString released = "font: 14pt \"MS Shell Dlg 2\";"
                        "color:white;"
                        "background-color:#79634E;"
                        "min-width:7em;"
                        "max-width:7em;"
                        "min-height:2.5em;"
                        "max-height:2.5em;";

        if(actual_widget=="page_2"){
           l_a->removeItem(l_a->itemAt(0));
            delete p2;
           ui->pushButton->setStyleSheet(released);
           ui->pushButton_2->setStyleSheet(pushed);
            p1 = new page_1(this);
            connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p1, SLOT(receiver(QString, QString, QString, QString)));
            emit send_info(this->userName, this->realName, this->token, this->url);
            l_a->addWidget (p1);
            actual_widget = "page_1";
        }
        else{
            ui->pushButton->setStyleSheet(released);
            ui->pushButton_2->setStyleSheet(pushed);
        }
}

void MainWindow::on_pushButton_2_clicked() {

    QString pushed = "font: 14pt \"MS Shell Dlg 2\";"
            "color:white;"
            "background-color:rgba(153,153,153,50%);"
            "min-width:7em;"
            "max-width:7em;"
            "min-height:2.5em;"
            "max-height:2.5em;";

    QString released = "font: 14pt \"MS Shell Dlg 2\";"
                    "color:white;"
                    "background-color:#79634E;"
                    "min-width:7em;"
                    "max-width:7em;"
                    "min-height:2.5em;"
                    "max-height:2.5em;";

    if(actual_widget=="page_1"){
        l_a->removeItem(l_a->itemAt(0));
        delete p1;
        ui->pushButton->setStyleSheet(pushed);
        ui->pushButton_2->setStyleSheet(released);
        p2 = new page_2(this);
        connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p2, SLOT(receiver(QString, QString, QString, QString)));
        emit send_info(this->userName, this->realName, this->token, this->url);

        l_a->addWidget (p2);
        actual_widget = "page_2";
    }
    else{
        ui->pushButton->setStyleSheet(pushed);
        ui->pushButton_2->setStyleSheet(released);
    }
}

void MainWindow::receive_info(QString userName, QString realName, QString token, QString url){

    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    emit send_next();

    l_a = new QVBoxLayout(this);
    p1 = new page_1(this);

    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),p1, SLOT(receiver(QString, QString, QString, QString)));
    emit send_info(this->userName, this->realName, this->token, this->url);

    l_a-> addWidget(p1);
    l_a ->setMargin (0);

    ui->widget->setLayout (l_a);
    actual_widget = "page_1";

}

void MainWindow::on_icon_back_clicked()
{
    ham = new Ham_options(this);
    connect(ham, SIGNAL(getOut()),this,SLOT(close_session()));
    connect(ham, SIGNAL(send_notebook()), this, SLOT(show_notebook()));
    connect(ham, SIGNAL(send_inout()),this, SLOT(show_inout()));
    ham -> show();
}

void MainWindow::close_session(){
        emit logOut();
//        delete l_a;
//        if (actual_widget == "page_1"){
//            delete p1;
//        }
//        else{
//            delete p2;
//        }
}

void MainWindow::show_notebook(){

    notebook = new Notebook(this);
    notebook->show();

}

void MainWindow::show_inout(){
    notebook->close();
}
