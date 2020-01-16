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

   // Set icons
  double pix_w_a = (width*100)/1920;
  double pix_h_a = (height*100)/1080;

  QPixmap main_pix(":/images/images/camino_real.png");
  ui->main_icon->setPixmap(main_pix.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_w_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->main_icon->setFixedSize(static_cast<int>(pix_w_a), static_cast<int>(pix_h_a));

   //set Icons
  double pix_w_b = (width*30)/1920;
  double pix_h_b = (height*30)/1080;
  QPixmap back_pix(":/images/images/hamburguesa.png");
  QIcon ButtonIcon(back_pix.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_back->setIcon(ButtonIcon);
  ui->icon_back->setIconSize(QSize(static_cast<int>(pix_w_b),static_cast<int>(pix_h_b)));

  double pix_w = width;
  double pix_h = (height*0.9);

  QPixmap pixmap(":/images/images/dark_background.jpg");
  QPalette palette;

  palette.setBrush(QPalette::Background, pixmap.scaled(static_cast<int>(pix_w),static_cast<int>(pix_h)));
  ui->frame_2->setAutoFillBackground(true);
  ui->frame_2->setPalette(palette);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receive_info(QString userName, QString realName, QString token, QString url){

    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    l_a = new QVBoxLayout(this);
    inOut = new InOut(this);

    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),inOut, SLOT(receive_info(QString, QString, QString, QString)));
    connect(inOut,SIGNAL(send_clients()),this,SLOT(show_clients()));
    emit send_info(this->userName, this->realName, this->token, this->url);

    l_a -> addWidget(inOut);
    l_a -> setMargin(0);

    ui -> widget -> setLayout(l_a);
    actual_widget = "inOut";

}

void MainWindow::on_icon_back_clicked()
{
    ham = new Ham_options(this);
    connect(ham, SIGNAL(getOut()),this,SLOT(close_session()));
    connect(ham, SIGNAL(send_notebook()), this, SLOT(show_notebook()));
    connect(ham, SIGNAL(send_inout()),this, SLOT(show_inout()));
    connect(ham, SIGNAL(send_clients()),this, SLOT(show_clients()));
    ham -> show();
}

void MainWindow::close_session(){
        emit logOut();
        delete l_a;
        if (actual_widget == "inOut"){
            delete inOut;
        }
        else if(actual_widget=="clients"){
            delete clients;
        }
        else{
            delete notebook;
        }
}

void MainWindow::show_notebook(){

//    notebook = new Notebook(this);
//    notebook->show();

    if(actual_widget=="inOut"){
        l_a->removeItem(l_a->itemAt(0));
        delete inOut;
     }
    else if (actual_widget == "clients"){
        l_a->removeItem(l_a->itemAt(0));
        delete clients;
    }
    else{
        l_a->removeItem(l_a->itemAt(0));
        delete notebook;
    }

    notebook = new Notebook(this);

    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),notebook, SLOT(receive_info(QString, QString, QString, QString)));
    emit send_info(this->userName, this->realName, this->token, this->url);

    l_a->addWidget (notebook);
    actual_widget = "notebook";

}

void MainWindow::show_inout(){

    if(actual_widget=="notebook"){
        l_a->removeItem(l_a->itemAt(0));
        delete notebook;
    }
    else if(actual_widget == "clients"){
        l_a->removeItem(l_a->itemAt(0));
        delete clients;
     }
    else{
        l_a->removeItem(l_a->itemAt(0));
        delete inOut;
    }

    inOut = new InOut(this);
    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),inOut, SLOT(receive_info(QString, QString, QString, QString)));
    connect(inOut,SIGNAL(send_clients()),this,SLOT(show_clients()));
    emit send_info(this->userName, this->realName, this->token, this->url);

    l_a->addWidget (inOut);
    actual_widget = "inOut";
}

void MainWindow::show_clients(){

    if(actual_widget=="notebook"){
        l_a->removeItem(l_a->itemAt(0));
        delete notebook;
    }
    else if(actual_widget == "inOut"){
        l_a->removeItem(l_a->itemAt(0));
        delete inOut;
     }
    else{
        l_a->removeItem(l_a->itemAt(0));
        delete clients;
    }

    clients = new Clients(this);
    connect(this, SIGNAL(send_info(QString, QString, QString, QString)),clients, SLOT(receive_info(QString, QString, QString, QString)));
    emit send_info(this->userName, this->realName, this->token, this->url);

    l_a->addWidget (clients);
    actual_widget = "clients";

}
