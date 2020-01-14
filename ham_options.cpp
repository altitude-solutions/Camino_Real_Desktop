#include "ham_options.h"
#include "ui_ham_options.h"
#include <QScreen>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

Ham_options::Ham_options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ham_options)
{
    ui->setupUi(this);
    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0] -> geometry().width();
   int height = screens[0] -> geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width);
    double size_h = (height);
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

    ui -> frame_2 -> setFixedWidth(static_cast<int>(width*0.1));

}

Ham_options::~Ham_options()
{
    delete ui;
}

void Ham_options::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

void Ham_options::mousePressEvent(QMouseEvent *event){
    qDebug()<<event;
    this ->close();
}

void Ham_options::on_pushButton_9_clicked()
{
      emit send_inout();
      this -> close();
}

void Ham_options::on_close_seesion_clicked()
{
    emit getOut();
    this -> close();
}

void Ham_options::on_pushButton_10_clicked()
{
    emit send_notebook();
    this -> close();
}
