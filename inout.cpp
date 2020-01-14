#include "inout.h"
#include "ui_inout.h"

InOut::InOut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InOut)
{
    ui->setupUi(this);
}

InOut::~InOut()
{
    delete ui;
}
