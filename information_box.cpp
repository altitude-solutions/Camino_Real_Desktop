#include "information_box.h"
#include "ui_information_box.h"
#include <QScreen>
#include <QDebug>

Information_box::Information_box(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Information_box)
{
    ui->setupUi(this);

}

Information_box::~Information_box()
{
    delete ui;
}

void Information_box::receive_info(QString icon, QString header, QString text, double size_w, double size_h){

    qDebug()<<icon;
    ui -> header -> setText(header);
    ui -> main_text -> setText(text);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();

   if(size_w<width){
       ui -> frame_2 -> setFixedWidth(static_cast<int>(size_w*0.45));
       ui -> frame -> setFixedWidth(static_cast<int>(size_w*0.45));
   }
   else{
       ui -> frame_2 -> setFixedWidth(static_cast<int>(size_w*0.2));
       ui -> frame -> setFixedWidth(static_cast<int>(size_w*0.2));
   }

    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

}

void Information_box::on_pushButton_14_clicked()
{
    ui -> header -> setText("");
    ui -> main_text -> setText("");
    this->close();
}
