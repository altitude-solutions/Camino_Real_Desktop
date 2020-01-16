#include "clients.h"
#include "ui_clients.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QJsonDocument>

Clients::Clients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clients)
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

    //Frames size:
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/1.45));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.8));


    //Set the table Size
    ui -> table_clients -> setColumnCount(9);
    ui->table_clients ->setColumnWidth(0,70);
    ui->table_clients ->setColumnWidth(1,static_cast<int>(width/14));  //Cliente
    ui->table_clients ->setColumnWidth(2,static_cast<int>(width/14));  //Regional
    ui->table_clients ->setColumnWidth(3,static_cast<int>(width/8));  //Nombre del contacto
    ui->table_clients ->setColumnWidth(4,static_cast<int>(width/13));  //Teléfono del contacto
    ui->table_clients ->setColumnWidth(5,static_cast<int>(width/11));  //E-mail del contacto
    ui->table_clients ->setColumnWidth(6,static_cast<int>(width/12));  //Cargo del contacto
    ui->table_clients ->setColumnWidth(7,static_cast<int>(width/8));  // Asesor de ventas
    ui->table_clients ->setColumnWidth(8,0);

    //Setting the table headers
    QStringList headers = {"",
                           "Cliente",
                           "Regional",
                           "Contacto",
                           "Teléfono",
                           "E-mail",
                           "Cargo",
                           "Asesor"};

    ui -> table_clients -> setHorizontalHeaderLabels(headers);

}

Clients::~Clients()
{
    delete ui;
}

void Clients::receive_info(QString userName, QString realName, QString token, QString url){

    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;
    read_client_info();
    db_clients.clear();
    contact.clear();
}

void Clients::read_client_info()
{
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            information_box("x","Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        foreach (QJsonValue entidad, okJson.object ().value ("clients").toArray ()) {
            foreach (QJsonValue regional, entidad.toObject().value("regionals").toArray()) {
                foreach( QJsonValue contacto, regional.toObject().value("contacts").toArray()){

                    QHash<QString, QString> current;
                    current.insert ("contact", contacto.toObject ().value ("name").toString ());
                    current.insert ("job", contacto.toObject ().value ("job").toString());
                    current.insert ("phone", contacto.toObject ().value ("phoneNumbers").toArray().at(0).toString());
                    current.insert ("email", contacto.toObject ().value ("emailAddresses").toArray().at(0).toString());
                    current.insert ("id_contacto", contacto.toObject ().value ("_id").toString());

                    current.insert("regional",regional.toObject ().value("city").toString());
                    current.insert("id_regional",regional.toObject ().value("_id").toString());
                    current.insert("category",regional.toObject ().value("category").toString());

                    current.insert("agent_id",regional.toObject ().value("salesAgent").toObject().value("_id").toString());
                    current.insert("user_name",regional.toObject ().value("salesAgent").toObject().value("userName").toString());
                    current.insert("real_name",regional.toObject ().value("salesAgent").toObject().value("realName").toString());

                    current.insert("id_cliente",entidad.toObject ().value("_id").toString());
                    current.insert("client",entidad.toObject ().value("name").toString());

                    db_clients.insert(contacto.toObject ().value("_id").toString(), current);
                }
             }
          }
        update_table(db_clients);
        reply->deleteLater ();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/clients?from=0&to=1000&status=1"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void Clients::information_box(QString icon, QString header, QString text)
{
    box_info = new Information_box(this);
    connect(this, SIGNAL(send_info_box(QString, QString, QString, double, double)),box_info, SLOT(receive_info(QString,QString, QString, double, double)));

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double w = (static_cast<int>(width));
    double h = (static_cast<int>(height*0.9));

    emit send_info_box(icon, header, text, w, h);
    box_info->show();
}

void Clients::update_table(QHash<QString, QHash<QString, QString>> update){
    //Rewrite the local table
    ui -> table_clients -> setRowCount(0);

    QHashIterator<QString, QHash<QString, QString>>iter(update);
    ui->table_clients->setSortingEnabled(false);
    while(iter.hasNext()){

        auto current = iter.next().key();

        //Add a new row
        int  row_control;
        ui->table_clients->insertRow(ui->table_clients->rowCount());
        row_control= ui->table_clients->rowCount()-1;

        //Writing the current row
        ui->table_clients->setItem(row_control, 0, new QTableWidgetItem(QString::number(row_control)));
        ui->table_clients->setItem(row_control, 1, new QTableWidgetItem(update[current]["client"]));
        ui->table_clients->setItem(row_control, 2, new QTableWidgetItem(update[current]["regional"]));
        ui->table_clients->setItem(row_control, 3, new QTableWidgetItem(update[current]["contact"]));
        ui->table_clients->setItem(row_control, 4, new QTableWidgetItem(update[current]["phone"]));
        ui->table_clients->setItem(row_control, 5, new QTableWidgetItem(update[current]["email"]));
        ui->table_clients->setItem(row_control, 6, new QTableWidgetItem(update[current]["job"]));
        ui->table_clients->setItem(row_control, 7, new QTableWidgetItem(update[current]["real_name"]));
        ui->table_clients->setItem(row_control, 8, new QTableWidgetItem(current));

    }
}

void Clients::on_table_clients_cellClicked(int row, int column)
{
    qDebug()<<column;
    contact["client"] = ui -> table_clients -> item(row,1)->text();
    contact["regional"] = ui -> table_clients -> item(row,2)->text();
    contact["contact"] = ui -> table_clients -> item(row,3)->text();
    contact["phone"] = ui -> table_clients -> item(row,4)->text();
    contact["email"] = ui -> table_clients -> item(row,5)->text();
    contact["job"] = ui -> table_clients -> item(row,6)->text();
    contact["real_name"] = ui -> table_clients -> item(row,7)->text();
    contact["id_contacto"] = ui -> table_clients -> item(row,8)->text();

    paint_table(row);
}

void Clients::paint_table(int row){

    for (int i=0; i<db_clients.size(); i++){
        if (i==row){
            ui->table_clients->item(row,0)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,1)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,2)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,3)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,4)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,5)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,6)->setBackground(QColor("#F2F2F2"));
            ui->table_clients->item(row,7)->setBackground(QColor("#F2F2F2"));
        }
        else {
            ui->table_clients->item(i,0)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,1)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,2)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,3)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,4)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,5)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,6)->setBackground(QColor("#FFFFFF"));
            ui->table_clients->item(i,7)->setBackground(QColor("#FFFFFF"));
        }
    }
}

void Clients::on_modify_butt_clicked()
{
    if(contact["contact"]!=""){
        modify = new Modify(this);
        connect(this, SIGNAL(send_contact(QHash<QString,QString>,QString, QString)),modify,SLOT(receive_contact(QHash<QString,QString>,QString, QString)));
        connect(modify,SIGNAL(send_update()),this,SLOT(update_client()));
        emit send_contact(contact, this->token, this -> url);
        modify->show();
    }
    else{
        information_box("x","Error","Seleccionar un registro porfavor");
    }
}

void Clients::on_new_butt_clicked()
{
    new_client = new New(this);
    connect(this, SIGNAL(send_info(QString, QString)),new_client,SLOT(receive_info(QString, QString)));
    connect(new_client,SIGNAL(send_update()),this,SLOT(update_client()));
    emit send_info(this->token, this -> url);
    new_client->show();
}

void Clients::update_client(){
    information_box("x","Base de datos", "Actualizado con éxito");
    read_client_info();
}
