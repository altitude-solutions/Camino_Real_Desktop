#include "extrainfo.h"
#include "ui_extrainfo.h"
#include <QScreen>

ExtraInfo::ExtraInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtraInfo)
{
    ui->setupUi(this);
    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width)/2;
    double size_h = (height)/1.8;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

}

ExtraInfo::~ExtraInfo()
{
    delete ui;
}

void ExtraInfo::on_pushButton_9_clicked()
{
    QString competencia = ui -> textEdit-> toPlainText();
    QString tarifa = ui -> textEdit_2-> toPlainText();
    QString noches = ui -> textEdit_3-> toPlainText();

    emit send_info(competencia, tarifa, noches);
    ui -> textEdit -> setPlainText("");
    ui -> textEdit_2 -> setPlainText("");
    ui -> textEdit_3 -> setPlainText("");
    this->close();
}
