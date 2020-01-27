#include "new.h"
#include "ui_new.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCompleter>

New::New(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::New)
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

    //Frames size
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/2.5));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.9));

    ui -> frame_2 -> setFixedHeight(static_cast<int>(height*0.05));
    ui -> frame_6 -> setFixedHeight(static_cast<int>(height*0.05));

}

New::~New()
{
    delete ui;
}

void New::on_pushButton_clicked()
{

    this -> close();
}

void New::receive_info(QString token, QString url){
    this -> token = token;
    this -> url = url;

    set_completer();
    set_clients();
    set_agents();
}

void New::on_tarifario_butt_clicked()
{
    //Contacto info
    QString name = ui -> nombre -> text();
    QString phone = ui -> telefono -> text();
    QString mail = ui -> mail -> text();
    QString job = ui -> cargo -> text();

    QJsonDocument document;
    QStringList saved;
    QJsonObject contacto;
    QJsonObject cliente;
    QJsonObject regional;
    QJsonObject main_object;
    QJsonArray phones;
    QJsonArray mails;

    QString client = ui -> cliente -> text();
    QString regions = ui -> sucursal -> text();

    if(phone!=""){
        phones.append(phone);
    }
    if(mail!=""){
        mails.append(mail);
    }
    contacto.insert("emailAddresses",mails);
    contacto.insert("phoneNumbers",phones);
    if (name==""){
        name = "-";
    }
    if(job==""){
        job = "-";
    }

    contacto.insert("name",name);
    contacto.insert("job", job);

    regional.insert("city",regions);

    cliente.insert("name",client);

    main_object.insert("contact",contacto);
    main_object.insert("regional",regional);
    main_object.insert("client",cliente);

    document.setObject(main_object);

   //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);
    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
        QByteArray binReply = reply->readAll ();
        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
            if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                QMessageBox::critical (this, "Error", QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
                QMessageBox::critical (this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            emit send_update();
            this -> close();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/app_clients"));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson ());
}

void New::set_completer()
{
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            QMessageBox::critical(this,"Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        QStringList completer_list;

        foreach (QJsonValue ciudades, okJson.object ().value ("cities").toArray ()) {
            tabla_ciudades[ciudades.toObject().value("city").toString()] = ciudades.toObject().value("_id").toString();
            completer_list << ciudades.toObject().value("city").toString();
          }

        completer_list.removeDuplicates();
        std::sort(completer_list.begin(), completer_list.end());
        QCompleter *client_completer = new QCompleter(completer_list,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        ui -> sucursal -> setCompleter(client_completer);
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/cities"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);

}

void New::set_clients()
{
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            QMessageBox::critical(this,"Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        QStringList completer_list;

        foreach (QJsonValue clientes, okJson.object ().value ("clients").toArray ()) {
            tabla_clientes[clientes.toObject().value("name").toString()] = clientes.toObject().value("_id").toString();
            completer_list << clientes.toObject().value("name").toString();
          }

        completer_list.removeDuplicates();
        std::sort(completer_list.begin(), completer_list.end());
        QCompleter *client_completer = new QCompleter(completer_list,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        ui -> cliente -> setCompleter(client_completer);
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/clients"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void New::set_agents()
{
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            QMessageBox::critical(this,"Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        QStringList completer_list;

        foreach (QJsonValue ciudades, okJson.object ().value ("users").toArray ()) {
            tabla_agentes[ciudades.toObject().value("realName").toString()] = ciudades.toObject().value("_id").toString();
            completer_list << ciudades.toObject().value("realName").toString();
          }

        completer_list.removeDuplicates();
        std::sort(completer_list.begin(), completer_list.end());
        QCompleter *client_completer = new QCompleter(completer_list,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        //ui -> sucursal -> setCompleter(client_completer);
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/users"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}


void New::on_cliente_editingFinished()
{
    QString client = ui -> cliente -> text();
    if(client!=""){
        if(tabla_clientes[client]==""){
            this -> client_flag = client;

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Cliente inexistente", "El cliente " +client+ " no se encuentra registrado en la base de datos\n"
                                                                         "Desea crear un nuevo cliente con ese nombre?",QMessageBox::Yes|QMessageBox::No);
            ui -> cliente -> setText("");
            if(reply == QMessageBox::Yes){
                create_client(client);
            }
            else{
                ui->cliente->setText("");
            }
        }
        else{
            this -> id_cliente =  tabla_clientes[client];
        }
    }
}

void New::create_client(QString client_name){

    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;

    main_object.insert("name", client_name);
    document.setObject(main_object);

    //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);
    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
        QByteArray binReply = reply->readAll ();
        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
            if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                QMessageBox::critical(this,"Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
                QMessageBox::critical(this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            QJsonDocument receiver = QJsonDocument::fromJson(binReply);

            this -> id_cliente = receiver.object().value("client").toObject().value("_id").toString();
            set_clients();
            ui -> cliente -> setText(this -> client_flag);
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/clients"));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson ());
}


//Creating new city and regional
void New::on_sucursal_editingFinished()
{
    QString city = ui -> sucursal -> text();
    if(city!=""){
        if(tabla_ciudades[city]==""){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Ciudad inexistente", "La ciudad " +city+ " no se encuentra registrada en la base de datos\n"
                                                                         "Desea crear una nueva ciudad con ese nombre?",QMessageBox::Yes|QMessageBox::No);
            if(reply == QMessageBox::Yes){
                create_city(city);
            }
            else{
                ui->sucursal->setText("");
            }
        }
        else{
            this -> id_ciudad = tabla_ciudades[city];
        }
    }
}

void New::create_city(QString city_name){
    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;

    main_object.insert("city", city_name);
    document.setObject(main_object);

    //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);
    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
        QByteArray binReply = reply->readAll ();
        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
            if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                QMessageBox::critical(this,"Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
                QMessageBox::critical(this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            QJsonDocument receiver = QJsonDocument::fromJson(binReply);
            this -> id_ciudad = receiver.object().value("city").toObject().value("city").toString();
            qDebug()<<id_ciudad;
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/cities/"+city_name));
    request.setRawHeader ("token", this -> token.toUtf8());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson ());
}

