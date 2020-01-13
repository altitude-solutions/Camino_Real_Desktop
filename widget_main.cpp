#include "widget_main.h"
#include "ui_widget_main.h"
#include <QPalette>
#include <QPixmap>

widget_main::widget_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_main)
{
    ui->setupUi(this);

     QPixmap pixmap(":/images/images/dark_background.jpg");
    pixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Background, pixmap);

    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

widget_main::~widget_main()
{
    delete ui;
}
