#include "lead_information.h"
#include "ui_lead_information.h"
#include <QScreen>

Lead_information::Lead_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lead_information)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width);
    double size_h = (height);
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);
}

Lead_information::~Lead_information()
{
    delete ui;
}

void Lead_information::on_pushButton_9_clicked(){
    QString restaurante = ui -> restaurantes -> toPlainText();
    QString eventos = ui -> eventos -> toPlainText();
    QString other = ui -> other -> toPlainText();

    emit send_lead(restaurante, eventos, other);
    this -> close();
}
