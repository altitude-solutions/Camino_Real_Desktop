#include "clients_list.h"
#include "ui_clients_list.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCompleter>

Clients_list::Clients_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clients_list)
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
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/1.76));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.8));

    //Set the table Size
    ui -> table_clients_2 -> setColumnCount(6);
    ui->table_clients_2 ->setColumnWidth(0,static_cast<int>(width/9));  //Cliente
    ui->table_clients_2 ->setColumnWidth(1,static_cast<int>(width/9));  //Regional
    ui->table_clients_2 ->setColumnWidth(2,static_cast<int>(width/9));  //Nombre del contacto
    ui->table_clients_2 ->setColumnWidth(3,static_cast<int>(width/9));  //Teléfono del contacto
    ui->table_clients_2 ->setColumnWidth(4,static_cast<int>(width/9));  //E-mail del contacto
    ui->table_clients_2 ->setColumnWidth(5,0);

    //Setting the table headers
    QStringList headers = {"Cliente",
                           "Regional",
                           "Aniversario",
                           "Categoría",
                           "Agente"};

    ui -> table_clients_2 -> setHorizontalHeaderLabels(headers);
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

    //Read main Info
    read_client_info("");
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
    if(this->cliente["Client"]!=""){
        update = new Update_client(this);

        connect(this, SIGNAL(send_contact(QHash<QString,QString>,QString, QString)),update,SLOT(receive_contact(QHash<QString,QString>,QString, QString)));
        connect(update,SIGNAL(send_update()),this,SLOT(update_client()));

        emit send_contact(cliente, this->token, this -> url);

        update->show();
    }
    else{
        QMessageBox::warning(this,"Error","Seleccionar un registro porfavor");
    }
}

void Clients_list::read_client_info(QString filter)
{
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            QMessageBox::warning(this,"Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        QStringList completer;

        tabla_general.clear();

        foreach (QJsonValue entidad, okJson.object ().value ("clients").toArray ()) {
            QString cliente = entidad.toObject ().value("name").toString();
            QString id_cliente = entidad.toObject ().value("_id").toString();

            completer << cliente;
            foreach (QJsonValue regional, entidad.toObject().value("regionals").toArray()) {

                QString ciudad = regional.toObject().value("city").toObject().value("city").toString();
                QString id_ciudad = regional.toObject().value("city").toObject().value("_id").toString();
                QString id_sucursal = regional.toObject().value("_id").toString();

                QString categoria = regional.toObject().value("category").toObject().value("category").toString();
                QString id_categoria = regional.toObject().value("category").toObject().value("_id").toString();

                QDateTime time = QDateTime::fromMSecsSinceEpoch(regional.toObject().value("anniversary").toVariant().toLongLong());
                QString aniversario = time.toString("dd/MM/yyyy");

                QString agente = regional.toObject().value("salesAgent").toObject().value("realName").toString();
                QString id_agente = regional.toObject().value("salesAgent").toObject().value("_id").toString();

                this -> tabla_general[id_sucursal]["Cliente"] = cliente;
                this -> tabla_general[id_sucursal]["id_cliente"] = id_cliente;
                this -> tabla_general[id_sucursal]["Ciudad"] = ciudad;
                this -> tabla_general[id_sucursal]["id_ciudad"] = id_ciudad;
                this -> tabla_general[id_sucursal]["Aniversario"] = aniversario;
                this -> tabla_general[id_sucursal]["Categoria"] = categoria;
                this -> tabla_general[id_sucursal]["id_categoria"] = id_categoria;
                this -> tabla_general[id_sucursal]["Agente"] = agente;
                this -> tabla_general[id_sucursal]["id_agente"] = id_agente;
              }
          }

        completer.removeDuplicates();
        std::sort(completer.begin(), completer.end());
        QCompleter *client_completer = new QCompleter(completer,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        ui -> lineEdit_2 -> setCompleter(client_completer);

        update_table(tabla_general);
        reply->deleteLater ();
    });
    QString filter_data = "";

    if(filter != ""){
        filter_data = "&q="+filter;
    }

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/clients?from=0&to=1000&status=1"+filter_data));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void Clients_list::update_table(QHash<QString, QHash<QString, QString>> update){
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
        ui->table_clients_2->setItem(row_control, 0, new QTableWidgetItem(tabla_general[current]["Cliente"]));
        ui->table_clients_2->setItem(row_control, 1, new QTableWidgetItem(tabla_general[current]["Ciudad"]));
        ui->table_clients_2->setItem(row_control, 2, new QTableWidgetItem(tabla_general[current]["Aniversario"]));
        ui->table_clients_2->setItem(row_control, 3, new QTableWidgetItem(tabla_general[current]["Categoria"]));
        ui->table_clients_2->setItem(row_control, 4, new QTableWidgetItem(tabla_general[current]["Agente"]));
        ui->table_clients_2->setItem(row_control, 5, new QTableWidgetItem(current));

    }
   ui -> table_clients_2 -> setSortingEnabled(true);
}

void Clients_list::on_table_clients_2_cellClicked(int row, int column)
{
    QString Sucursal_ID =  ui -> table_clients_2 -> item(row,5)->text();
    qDebug()<<column;

    //Gral Information
    this -> cliente["Client"] = ui -> table_clients_2 -> item(row,0)->text();
    this -> cliente["City"] = ui -> table_clients_2 -> item(row,1)->text();
    this -> cliente["Anniversary"] = ui -> table_clients_2 -> item(row,2)->text();
    this -> cliente["Category"] = ui -> table_clients_2 -> item(row,3)->text();
    this -> cliente["Agent"] = ui -> table_clients_2 -> item(row,4)->text();

    //Id's
    this -> cliente["sucursal_id"] = Sucursal_ID;
    this -> cliente["client_id"] = this -> tabla_general[Sucursal_ID]["id_cliente"];
    this -> cliente["id_ciudad"] = this -> tabla_general[Sucursal_ID]["id_ciudad"];
    this -> cliente["id_categoria"] = this -> tabla_general[Sucursal_ID]["id_categoria"];
    this -> cliente["id_agente"] = this -> tabla_general[Sucursal_ID]["id_agente"];

    paint_table(row);
}

void Clients_list::paint_table(int row){

    for (int i=0; i<tabla_general.size(); i++){
        if (i==row){
            ui->table_clients_2->item(row,0)->setBackground(QColor("#B6B6B6"));
            ui->table_clients_2->item(row,1)->setBackground(QColor("#B6B6B6"));
            ui->table_clients_2->item(row,2)->setBackground(QColor("#B6B6B6"));
            ui->table_clients_2->item(row,3)->setBackground(QColor("#B6B6B6"));
            ui->table_clients_2->item(row,4)->setBackground(QColor("#B6B6B6"));

        }
        else {
            ui->table_clients_2->item(i,0)->setBackground(QColor("#FFFFFF"));
            ui->table_clients_2->item(i,1)->setBackground(QColor("#FFFFFF"));
            ui->table_clients_2->item(i,2)->setBackground(QColor("#FFFFFF"));
            ui->table_clients_2->item(i,3)->setBackground(QColor("#FFFFFF"));
            ui->table_clients_2->item(i,4)->setBackground(QColor("#FFFFFF"));
        }
    }
}

void Clients_list::on_delete_butt_2_clicked(){

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Eliminar contacto", "Seguro desea remover este contacto de la base de datos?",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(cliente["client_id"]!=""){
            //Send information
            QNetworkAccessManager* nam = new QNetworkAccessManager (this);
            connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
                QByteArray binReply = reply->readAll ();
                if (reply->error ()) {
                    QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
                    if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                        QMessageBox::warning(this,"Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
                    }
                    else {
                        QMessageBox::warning(this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
                    }
                }
                update_client();
                reply->deleteLater ();
            });

            QNetworkRequest request;
            request.setUrl (QUrl ("http://"+this -> url + "/regional_clients/"+cliente["sucursal_id"]));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");
            nam->sendCustomRequest(request,"DELETE");
        }
        else{
            QMessageBox::warning(this,"Seleccionar registro","Porfavor indicar el contacto que desea eliminar");
        }
    }
}

void Clients_list::update_client(){
    QMessageBox::warning(this, "Base de datos", "Sincronización con base de datos exitosa");
    read_client_info(ui ->lineEdit_2 -> text());
}

void Clients_list::on_lineEdit_2_editingFinished(){
    QString query  = ui -> lineEdit_2 -> text();
    read_client_info(query);
}

