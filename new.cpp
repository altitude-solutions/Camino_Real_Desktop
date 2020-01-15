#include "new.h"
#include "ui_new.h"
#include <QScreen>

New::New(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::New)
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

    //Frames size
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/2.5));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/2));

    ui -> frame_2 -> setFixedHeight(static_cast<int>(height*0.05));

}

New::~New()
{
    delete ui;
}

void New::on_pushButton_clicked()
{

    this -> close();
}
