#include "clients_list.h"
#include "ui_clients_list.h"

Clients_list::Clients_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clients_list)
{
    ui->setupUi(this);
}

Clients_list::~Clients_list()
{
    delete ui;
}

void Clients_list::receive_info(QString, QString, QString, QString)
{
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

}
