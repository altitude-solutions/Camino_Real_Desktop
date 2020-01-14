#include "notebook.h"
#include "ui_notebook.h"
#include <QScreen>

Notebook::Notebook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notebook)
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
    this->move(0,static_cast<int>(height*0.1));

    //Set frames
    ui->frame_2->setFixedHeight(static_cast<int>(height*0.9));

   double pix_w = width;
   double pix_h = (height*0.9);

   QPixmap pixmap(":/images/images/dark_background.jpg");
   QPalette palette;

   palette.setBrush(QPalette::Background, pixmap.scaled(static_cast<int>(pix_w),static_cast<int>(pix_h)));
   ui->frame_2->setAutoFillBackground(true);
   ui->frame_2->setPalette(palette);

}

Notebook::~Notebook()
{
    delete ui;
}
