#include "reservas.h"
#include "ui_reservas.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCompleter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <cmath>


Reservas::Reservas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reservas)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
   double size_w = (width)/1.2;
   double size_h = (height)/1.6;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

    //Frames size:
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/1.2));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.7));

    //Set the table Size
    ui -> table_clients_2 -> setColumnCount(10);
    ui->table_clients_2 ->setColumnWidth(0,static_cast<int>(width/15));  //Fecha
    ui->table_clients_2 ->setColumnWidth(1,static_cast<int>(width/6));  //Cliente
    ui->table_clients_2 ->setColumnWidth(2,static_cast<int>(width/8));  //Regional
    ui->table_clients_2 ->setColumnWidth(3,static_cast<int>(width/15)); // noches
    ui->table_clients_2 ->setColumnWidth(4,static_cast<int>(width/14));  //Early Check in
    ui->table_clients_2 ->setColumnWidth(5,static_cast<int>(width/14));  //Late Check Out
    ui->table_clients_2 ->setColumnWidth(6,static_cast<int>(width/14));  //Upgrade
    ui->table_clients_2 ->setColumnWidth(7,static_cast<int>(width/14));  //No show
    ui->table_clients_2 ->setColumnWidth(8,static_cast<int>(width/9.7));  //Agente
    ui->table_clients_2 ->setColumnWidth(9,0);

    //Setting the table headers
    QStringList headers = {"Fecha",
                           "Cliente",
                           "Regional",
                           "Noches",
                           "Early Check-in",
                           "Late Check-out",
                           "Upgrade",
                           "No Show",
                           "Agente"};

    ui -> table_clients_2 -> setHorizontalHeaderLabels(headers);
}

Reservas::~Reservas(){
    delete ui;
}

void Reservas::receiver(QString userName, QString realName, QString token, QString url){
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    read_info("0");
}

void Reservas::read_info(QString pages){

    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            send_info_box("x","Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        QStringList completer_list;

        tabla_informacion.clear();
        int registers;

        foreach (QJsonValue entidad, okJson.object ().value ("tasks").toArray ()) {
            QString todo = entidad.toObject().value("todo").toString();

            if(todo=="Reserva"){
                QString cliente = entidad.toObject ().value("client").toObject().value("name").toString();
                QString ciudad = entidad.toObject ().value("regional").toObject().value("city").toObject().value("city").toString();
                QString date = QDateTime::fromMSecsSinceEpoch(entidad.toObject ().value("registerDate").toVariant().toLongLong()).toString("yyyy/MM/dd");
                QString agente = entidad.toObject().value("creationAgent").toObject().value("realName").toString();

                if(entidad.toObject().contains("outData")){

                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["client"] = cliente;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["city"] = ciudad;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["date"] = date;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["agente"] = agente;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["noches"] = QString::number(entidad.toObject().value("outData").toObject().value("nights").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["early"] = QString::number(entidad.toObject().value("outData").toObject().value("earlyCheckIn").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["late"] = QString::number(entidad.toObject().value("outData").toObject().value("lateCheckOut").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["upgrade"] = QString::number(entidad.toObject().value("outData").toObject().value("upgrade").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["noshow"] = QString::number(entidad.toObject().value("outData").toObject().value("noShow").toInt());

                }
                else if(entidad.toObject().contains("inData")){

                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["client"] = cliente;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["city"] = ciudad;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["date"] = date;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["agente"] = agente;
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["noches"] = QString::number(entidad.toObject().value("inData").toObject().value("nights").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["early"] = QString::number(entidad.toObject().value("inData").toObject().value("earlyCheckIn").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["late"] = QString::number(entidad.toObject().value("inData").toObject().value("lateCheckOut").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["upgrade"] = QString::number(entidad.toObject().value("inData").toObject().value("upgrade").toInt());
                    this -> tabla_informacion[entidad.toObject().value("_id").toString()]["noshow"] = QString::number(entidad.toObject().value("inData").toObject().value("noShow").toInt());

                }
            }
        }

        if(this->first_time){
            registers = okJson.object().value("count").toInt();
            define_pages(registers);
        }

        update_table(tabla_informacion);

        reply->deleteLater();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/tasks?todo=Reserva&deleted=0&from="+pages+"&to=50"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void Reservas::define_pages(int registers){
    int n_pages = 0;
    int defined_pages = 50;
    n_pages = std::ceil(registers / defined_pages) + 1;

    ui -> pages -> clear();
    for(int i = 1; i<n_pages+1; i++){
        ui -> pages -> addItem(QString::number(i));
    }

    this -> first_time = false;
}

void Reservas::on_pages_activated(const QString &arg1){
    int index = arg1.toInt();
    read_info(QString::number( ( index - 1) * 50 ) );
}

void Reservas::update_table(QHash<QString, QHash<QString, QString>>update){

    //Rewrite the local table
    ui -> table_clients_2 -> setRowCount(0);

    QHashIterator<QString, QHash<QString, QString>>iter(update);
    ui->table_clients_2->setSortingEnabled(false);
    while(iter.hasNext()){

        auto current = iter.next().key();

        //Add a new row
        int  row_control;
        ui->table_clients_2->insertRow(ui->table_clients_2->rowCount());
        row_control= ui->table_clients_2->rowCount()-1;

        //Writing the current row
        ui->table_clients_2->setItem(row_control, 0, new QTableWidgetItem(update[current]["date"]));
        ui->table_clients_2->setItem(row_control, 1, new QTableWidgetItem(update[current]["client"]));
        ui->table_clients_2->setItem(row_control, 2, new QTableWidgetItem(update[current]["city"]));
        ui->table_clients_2->setItem(row_control, 3, new QTableWidgetItem(update[current]["noches"]));
        ui->table_clients_2->setItem(row_control, 4, new QTableWidgetItem(update[current]["early"]));
        ui->table_clients_2->setItem(row_control, 5, new QTableWidgetItem(update[current]["late"]));
        ui->table_clients_2->setItem(row_control, 6, new QTableWidgetItem(update[current]["upgrade"]));
        ui->table_clients_2->setItem(row_control, 7, new QTableWidgetItem(update[current]["noshow"]));
        ui->table_clients_2->setItem(row_control, 8, new QTableWidgetItem(update[current]["agente"]));
        ui->table_clients_2->setItem(row_control, 9, new QTableWidgetItem(current));

    }
    ui -> table_clients_2 -> setSortingEnabled(true);
}

void Reservas::updater(){
     read_info("0");
}

void Reservas::on_table_clients_2_cellClicked(int row, int column){
    qDebug()<<column;
    this -> nights = ui->table_clients_2->item(row, 3)->text();

    this -> early = ui -> table_clients_2 -> item(row, 4)->text();
    this -> late = ui -> table_clients_2 -> item(row, 5)->text();
    this -> upgrade = ui -> table_clients_2 -> item(row, 6)->text();
    this -> noshow = ui -> table_clients_2 -> item(row, 7)->text();
    this -> id_register = ui -> table_clients_2 -> item(row, 9)->text();
}

void Reservas::on_modify_clicked(){
    if(id_register!=""){
        emit send_benefits_id(this -> id_register, this -> early, this -> late, this -> upgrade, this -> noshow, this -> nights);
    }
    else{
        send_info_box("x","Error","Seleccionar un registro porfavor");
    }
}

void Reservas::on_delete_2_clicked(){
    if(id_register!=""){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Eliminar reserva", "Seguro desea eliminar este registro?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            //create a Json
            QJsonDocument document;
            QJsonObject main_object;

            main_object.insert("deleted",true);

            document.setObject(main_object);

            //Send information
            QNetworkAccessManager* nam = new QNetworkAccessManager (this);
            connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
                QByteArray binReply = reply->readAll ();
                if (reply->error ()) {
                    QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
                    if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                        send_info_box("x","Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
                    } else {
                        send_info_box("x", "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
                    }
                }
                else{
                    read_info("0");
                }
                reply->deleteLater ();
            });

            QNetworkRequest request;
            request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+this->id_register));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");

            nam->put (request, document.toJson ());

            updater();
        }
    }
    else{
        send_info_box("x","Error","Seleccionar un registro porfavor");
    }
}
