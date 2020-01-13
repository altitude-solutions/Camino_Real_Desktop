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

    l_a = new QVBoxLayout(this);
    l_b = new QVBoxLayout(this);

    p1 = new page_1(this);
    l_a-> addWidget(p1);
    l_a ->setMargin (0);

    ui->widget->setLayout (l_a);
    actual_widget = "page_1";

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

   //   //Set icons
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

        QString pushed = "border-style:outset;"
                "font: 14pt \"MS Shell Dlg 2\";"
                "color:white;"
                "background-color:#999999;"
                "min-width:7em;"
                "max-width:7em;"
                "min-height:2.5em;"
                "max-height:2.5em;";

        QString released = "border-style:outset;"
                        "font: 14pt \"MS Shell Dlg 2\";"
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
            l_a->addWidget (p1);
            actual_widget = "page_1";
        }
}

void MainWindow::on_pushButton_2_clicked() {

    QString pushed = "border-style:outset;"
            "font: 14pt \"MS Shell Dlg 2\";"
            "color:white;"
            "background-color:#999999;"
            "min-width:7em;"
            "max-width:7em;"
            "min-height:2.5em;"
            "max-height:2.5em;";

    QString released = "border-style:outset;"
                    "font: 14pt \"MS Shell Dlg 2\";"
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
        l_a->addWidget (p2);
        actual_widget = "page_2";
    }
}
