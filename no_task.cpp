#include "no_task.h"
#include "ui_no_task.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCompleter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <cmath>


No_task::No_task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::No_task)
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
    ui -> tabla_general -> setColumnCount(7);
    ui->tabla_general ->setColumnWidth(0,static_cast<int>(width/12));  //Fecha
    ui->tabla_general ->setColumnWidth(1,static_cast<int>(width/5));  //Cliente
    ui->tabla_general ->setColumnWidth(2,static_cast<int>(width/12));  //Regional
    ui->tabla_general ->setColumnWidth(3,static_cast<int>(width/15));  //Vía
    ui->tabla_general ->setColumnWidth(4,static_cast<int>(width/15));  //Motivo
    ui->tabla_general ->setColumnWidth(5,static_cast<int>(width/15));  //Resultado
    ui->tabla_general ->setColumnWidth(6,static_cast<int>(width/4.03));  //Comentarios

    //Setting the table headers
    QStringList headers = {"Fecha",
                           "Cliente",
                           "Regional",
                           "Vía",
                           "Motivo",
                           "Resultado",
                           "Comentarios"};

    ui -> tabla_general -> setHorizontalHeaderLabels(headers);

    QStringList b_box_headers = {"input","output"};
    foreach (QString item, b_box_headers) {
        ui -> b_Box -> addItem(item);
    }
}

No_task::~No_task(){
    delete ui;
}

void No_task::receiver(QString userName, QString realName, QString token, QString url){
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    read_info("input","0");
}

void No_task::read_info(QString query, QString pages){

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

        foreach (QJsonValue entidad, okJson.object ().value ("data").toArray ()) {

            QString cliente = entidad.toObject ().value("client").toObject().value("name").toString();
            QString ciudad = entidad.toObject ().value("regional").toObject().value("city").toObject().value("city").toString();
            QString via = entidad.toObject ().value("via").toString();
            QString motivo = entidad.toObject ().value("reason").toString();
            QString date = QDateTime::fromMSecsSinceEpoch(entidad.toObject ().value("date").toVariant().toLongLong()).toString("yyyy/MM/dd");
            QString comentarios;
            QString resultado;

            if(entidad.toObject ().contains("result")){
                resultado = entidad.toObject ().value("result").toString();
            }
            else{
                resultado = "";
            }

            if(entidad.toObject().contains("comments")){
                comentarios  =  entidad.toObject ().value("comments").toString();
            }
            else{
                comentarios = "";
            }

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
    request.setUrl (QUrl ("http://"+this->url+"/not_tasks/"+query+"?from="+pages+"&to=50"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void No_task::define_pages(int registers){
    int n_pages = 0;
    int defined_pages = 50;
    n_pages = std::ceil(registers / defined_pages) + 1;

    ui -> pages -> clear();
    for(int i = 1; i<n_pages+1; i++){
        ui -> pages -> addItem(QString::number(i));
    }

    this -> first_time = false;
}

void No_task::update_table(QHash<QString, QHash<QString, QString>>update){

    //Rewrite the local table
    ui -> tabla_general -> setRowCount(0);

    QHashIterator<QString, QHash<QString, QString>>iter(update);
    ui->tabla_general->setSortingEnabled(false);
    while(iter.hasNext()){

        auto current = iter.next().key();

        //Add a new row
        int  row_control;
        ui->tabla_general->insertRow(ui->tabla_general->rowCount());
        row_control= ui->tabla_general->rowCount()-1;

        //Writing the current row
        ui->tabla_general->setItem(row_control, 0, new QTableWidgetItem(update[current]["date"]));
        ui->tabla_general->setItem(row_control, 1, new QTableWidgetItem(update[current]["client"]));
        ui->tabla_general->setItem(row_control, 2, new QTableWidgetItem(update[current]["city"]));
        ui->tabla_general->setItem(row_control, 3, new QTableWidgetItem(update[current]["via"]));
        ui->tabla_general->setItem(row_control, 4, new QTableWidgetItem(update[current]["motivo"]));
        ui->tabla_general->setItem(row_control, 5, new QTableWidgetItem(update[current]["result"]));
        ui->tabla_general->setItem(row_control, 6, new QTableWidgetItem(update[current]["comments"]));

    }
   ui -> tabla_general -> setSortingEnabled(true);
}

void No_task::on_b_Box_activated(const QString &arg1){
    this -> first_time  = true;
    read_info(arg1,"0");
}

void No_task::on_pages_activated(const QString &arg1){
    int index = arg1.toInt();
    read_info(ui -> b_Box ->currentText(), QString::number( ( index - 1) * 50 ) );
}
