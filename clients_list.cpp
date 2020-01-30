#include "clients_list.h"
#include "ui_clients_list.h"
#include <QMessageBox>

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

void Clients_list::receive_info(QString userName, QString realName, QString token, QString url){

    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;
}

void Clients_list::on_new_butt_2_clicked()
{
    new_cl = new New_client(this);

    connect(this, SIGNAL(send_info(QString, QString)),new_cl,SLOT(receive_info(QString, QString)));
    connect(new_cl,SIGNAL(send_update()),this,SLOT(update_client()));

    emit send_info(this->token, this -> url);

    new_cl->show();
}

void Clients_list::on_modify_butt_2_clicked()
{
    QHash<QString,QString>contact;
    contact["contact"]="hoala";

    if(contact["contact"]!=""){

        update = new Update_client(this);

        connect(this, SIGNAL(send_contact(QHash<QString,QString>,QString, QString)),update,SLOT(receive_contact(QHash<QString,QString>,QString, QString)));
        connect(update,SIGNAL(send_update()),this,SLOT(update_client()));

        emit send_contact(contact, this->token, this -> url);

        update->show();
    }
    else{
        QMessageBox::warning(this,"Error","Seleccionar un registro porfavor");
    }
}

void Clients_list::update_client(){
    //Here we should restart the table
}

