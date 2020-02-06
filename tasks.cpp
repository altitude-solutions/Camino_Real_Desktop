#include "tasks.h"
#include "ui_tasks.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCompleter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

Tasks::Tasks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tasks)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width)/1.45;
    double size_h = (height)/1.6;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

    //Frames size:
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/1.6));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.7));

    //Set the table Size
    ui -> table_clients_2 -> setColumnCount(8);
    ui->table_clients_2 ->setColumnWidth(0,static_cast<int>(width/12));  //Fecha
    ui->table_clients_2 ->setColumnWidth(1,static_cast<int>(width/10));  //Cliente
    ui->table_clients_2 ->setColumnWidth(2,static_cast<int>(width/10));  //Regional
    ui->table_clients_2 ->setColumnWidth(3,static_cast<int>(width/15));  //Vía
    ui->table_clients_2 ->setColumnWidth(4,static_cast<int>(width/15));  //Motivo
    ui->table_clients_2 ->setColumnWidth(5,static_cast<int>(width/15));  //Resultado
    ui->table_clients_2 ->setColumnWidth(6,static_cast<int>(width/7));  //Comentarios
    ui->table_clients_2 ->setColumnWidth(7,0);

    //Setting the table headers
    QStringList headers = {"Fecha",
                           "Cliente",
                           "Regional",
                           "Vía",
                           "Motivo",
                           "Resultado",
                           "Comentarios"};

    ui -> table_clients_2 -> setHorizontalHeaderLabels(headers);

}

Tasks::~Tasks(){
    delete ui;
}

void Tasks::receiver(QString userName, QString realName, QString token, QString url){
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    read_info("0");
}

void Tasks::read_info(QString pages){

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

            QString var = "inData";
            QString resultado="";
            QString comentarios;

            if(entidad.toObject().contains("outData")){
                var = "outData";
                resultado = entidad.toObject ().value(var).toObject().value("via").toString();
            }

            QString cliente = entidad.toObject ().value("client").toObject().value("name").toString();
            QString ciudad = entidad.toObject ().value("regional").toObject().value("city").toObject().value("city").toString();
            QString via = entidad.toObject ().value(var).toObject().value("via").toString();
            QString motivo = entidad.toObject ().value(var).toObject().value("reason").toString();
            QString date = QDateTime::fromMSecsSinceEpoch(entidad.toObject ().value("registerDate").toVariant().toLongLong()).toString();
            bool completed = entidad.toObject().value("completed").toBool();
            bool deleted = entidad.toObject().value("deleted").toBool();

            if(entidad.toObject().contains("comments")){
                comentarios  =  entidad.toObject ().value("comments").toString();
            }
            else{
                comentarios = "";
            }

            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["completed"] = completed;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["deleted"] = deleted;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["client"] = cliente;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["city"] = ciudad;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["via"] = via;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["motivo"] = motivo;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["date"] = date;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["result"] = resultado;
            this -> tabla_informacion[entidad.toObject().value("_id").toString()]["comments"] = comentarios;
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
    request.setUrl (QUrl ("http://"+this->url+"/tasks?from="+pages+"&to=50"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void Tasks::define_pages(int registers){
    int n_pages;
    int defined_pages = 50;
    ui -> pages -> clear();
    if(registers <= defined_pages){
        n_pages = 1;
    }
    else{
        if(registers%defined_pages!=0){
            n_pages = (registers/defined_pages)+1;
        }
        else{
            n_pages = registers/defined_pages;
        }
    }
    for(int i = 1; i<n_pages+1; i++){
        ui -> pages -> addItem(QString::number(i));
    }

    this -> first_time = false;
}

void Tasks::update_table(QHash<QString, QHash<QString, QString>>update){

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
        ui->table_clients_2->setItem(row_control, 3, new QTableWidgetItem(update[current]["via"]));
        ui->table_clients_2->setItem(row_control, 4, new QTableWidgetItem(update[current]["motivo"]));
        ui->table_clients_2->setItem(row_control, 5, new QTableWidgetItem(update[current]["result"]));
        ui->table_clients_2->setItem(row_control, 6, new QTableWidgetItem(update[current]["comments"]));
        ui->table_clients_2->setItem(row_control, 7, new QTableWidgetItem(current));

        if(update[current]["completed"]!=false&&update[current]["deleted"]!=true){
            ui->table_clients_2->item(row_control,0)->setBackground(QColor("#F2AB3E"));
            ui->table_clients_2->item(row_control,1)->setBackground(QColor("#F2AB3E"));
            ui->table_clients_2->item(row_control,2)->setBackground(QColor("#F2AB3E"));
            ui->table_clients_2->item(row_control,3)->setBackground(QColor("#F2AB3E"));
            ui->table_clients_2->item(row_control,4)->setBackground(QColor("#F2AB3E"));
            ui->table_clients_2->item(row_control,5)->setBackground(QColor("#F2AB3E"));
            ui->table_clients_2->item(row_control,6)->setBackground(QColor("#F2AB3E"));
        }
        else if (update[current]["deleted"]!=false){
            ui->table_clients_2->item(row_control,0)->setBackground(QColor("#999999"));
            ui->table_clients_2->item(row_control,1)->setBackground(QColor("#999999"));
            ui->table_clients_2->item(row_control,2)->setBackground(QColor("#999999"));
            ui->table_clients_2->item(row_control,3)->setBackground(QColor("#999999"));
            ui->table_clients_2->item(row_control,4)->setBackground(QColor("#999999"));
            ui->table_clients_2->item(row_control,5)->setBackground(QColor("#999999"));
            ui->table_clients_2->item(row_control,6)->setBackground(QColor("#999999"));
        }
    }
    ui -> table_clients_2 -> setSortingEnabled(true);
}

void Tasks::on_pages_activated(const QString &arg1){
    int index = arg1.toInt();
    int sender;
    if(index<=1){
        sender = 0;
    }
    else{
        sender = (index-1)*50;
    }
    read_info(QString::number(sender));
}

void Tasks::on_table_clients_2_cellClicked(int row, int column){
    qDebug()<<column;
    this -> register_id = ui -> table_clients_2 -> item(row, 7)->text();
}

void Tasks::on_modify_butt_2_clicked(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Recuperar task", "Desea recuperar este registro",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(this->register_id!=""){
            //create a Json
            QJsonDocument document;
            QJsonObject main_object;

            main_object.insert("deleted",false);

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
            request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+this->register_id));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");

            nam->put (request, document.toJson ());
        }
        else{
            send_info_box("x","Error","Seleccionar un registro porfavor");
        }
    }
}
