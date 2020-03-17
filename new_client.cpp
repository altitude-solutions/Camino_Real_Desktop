#include "new_client.h"
#include "ui_new_client.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCompleter>

#include <QEventLoop>



New_client::New_client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::New_client)
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

    //  Set Images
   double pix_w_b = (width*40)/1366;
   double pix_h_b= (height*40)/768;

   QPixmap pix_contacto(":/images/images/cliente.png");
   ui->icon_cliente->setPixmap(pix_contacto.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_cliente->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_cargo(":/images/images/regional_blanco.png");
  ui->icon_regional->setPixmap(pix_cargo.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_regional->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_telefono(":/images/images/categoria_blanco.png");
  ui->icon_categoria->setPixmap(pix_telefono.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_categoria->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_email(":/images/images/cumpleanos_blanco.png");
  ui->icon_aniversario->setPixmap(pix_email.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_aniversario->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  //set frame sizes
  ui -> frame -> setFixedWidth(static_cast<int>(width*0.3));
  ui -> frame_6 -> setFixedWidth(static_cast<int>(width*0.3));
  ui -> frame_8 -> setFixedWidth(static_cast<int>(width*0.3));

}

New_client::~New_client()
{
    delete ui;
}

void New_client::receive_info(QString token, QString url){
    this -> token = token;
    this -> url = url;

    //set main information
    set_city();
    set_client();
    set_agents();
    set_category();
}

void New_client::on_cancel_butt_clicked(){
    this -> close();
}

void New_client::on_aniversario_editingFinished(){

    QString anniversary = ui -> aniversario -> text();
    if(anniversary!=""){
        qlonglong time_validator = QDateTime::fromString(anniversary,"dd/MM/yyyy").toMSecsSinceEpoch();
        if(time_validator==14400000){
            QMessageBox::warning(this,"Error","Ingresar un formato de fecha válido");
            ui -> aniversario -> setText("");
        }
    }
}

void New_client::set_city(){
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

        ui -> regional_2 -> setCompleter(client_completer);

    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/cities"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void New_client::set_client(){
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

            foreach (QJsonValue regionales, clientes.toObject().value("regionals").toArray()) {
                clientes_ciudad[clientes.toObject().value("name").toString()][regionales.toObject().value("city").toObject().value("city").toString()] = regionales.toObject().value("_id").toString();
                clientes_categoria[clientes.toObject().value("name").toString()][regionales.toObject().value("city").toObject().value("city").toString()] = regionales.toObject().value("category").toObject().value("category").toString();
            }
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

void New_client::set_agents(){
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
        foreach (QString item, completer_list) {
            ui -> agente -> addItem(item);
        }
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/users?status=1"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void New_client::set_category(){
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

        foreach (QJsonValue category, okJson.object ().value ("categories").toArray ()) {
            tabla_categorias[category.toObject().value("category").toString()] = category.toObject().value("_id").toString();
            completer_list << category.toObject().value("category").toString();
          }

        completer_list.removeDuplicates();
        std::sort(completer_list.begin(), completer_list.end());
        foreach (QString item, completer_list) {
            ui -> categoria -> addItem(item);
        }
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/categories"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void New_client::on_cliente_editingFinished(){
//    QString client = ui -> cliente -> text();
//    if(client!=""){
//        if(tabla_clientes[client]==""){
//            ui -> cliente -> setText("");

//            QMessageBox::StandardButton reply;
//            reply = QMessageBox::question(this, "Cliente inexistente", "El cliente " +client+ " no se encuentra registrado en la base de datos\n"
//                                                                         "Desea crear un nuevo cliente con ese nombre?",QMessageBox::Yes|QMessageBox::No);

//            if(reply == QMessageBox::Yes){
//                create_client(client);
//            }
//            else{
//                ui->cliente->setText("");
//            }
//        }
//    }
}

void New_client::create_client(QString client_name){
    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;
    QJsonArray regionals_array;

    main_object.insert("name", client_name);
    main_object.insert("regionals",regionals_array);
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
            ui -> cliente -> setText(receiver.object().value("client").toObject().value("name").toString());
            set_client();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/clients"));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson ());
}


void New_client::on_regional_2_editingFinished(){
    QString city = ui -> regional_2 -> text();
    if(city!=""){
        if(tabla_ciudades[city]==""){
            ui -> regional_2 -> setText("");
            QMessageBox::warning(this, "Ciudad inexistente", QString::fromLatin1("La ciudad ") + city + QString::fromLatin1(" no se encuentra registrada en la base de datos"));
            // Uncomment this to enable city creation
//            QMessageBox::StandardButton reply;
//            reply = QMessageBox::question(this, "Ciudad inexistente", "La ciudad " +city+ " no se encuentra registrada en la base de datos\n"
//                                                                         "Desea crear una nueva ciudad con ese nombre?",QMessageBox::Yes|QMessageBox::No);
//            if(reply == QMessageBox::Yes){
//                create_city(city);
//            }
//            else{
//                ui->regional_2->setText("");
//            }
        }
        else{
            if(clientes_ciudad[ui->cliente->text()][city]!=""){
                QMessageBox::warning(this, "Error", "El cliente "+ui -> cliente -> text() + " ya cuenta con una sucursal en "+city);
                ui -> regional_2 -> setText("");
            }
        }
    }
}

void New_client::create_city(QString city){

    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;

    main_object.insert("city", city);
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
                QMessageBox::critical(this, "Error en base de datos", "Reportar este error porfavor" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            QJsonDocument receiver = QJsonDocument::fromJson(binReply);
            ui -> regional_2 -> setText(receiver.object().value("city").toObject().value("city").toString());
            set_city();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/cities"));
    request.setRawHeader ("token", this -> token.toUtf8());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson());
}

void New_client::on_guardar_butt_clicked(){
//    QString client = ui -> cliente -> text();
//    QString city = ui -> regional_2 -> text();
//    if(client!="" && city!=""){
//        ui -> guardar_butt -> setDisabled(true);
//        create_regional();
//    }
//    else{
//        QMessageBox::warning(this, "Error", "Seleccionar un cliente y sucursal porfavor");
//    }


    QString client = ui -> cliente -> text();
    QString city = ui -> regional_2 -> text();
    if(client!="" && city!="") {
        ui -> guardar_butt -> setDisabled(true);
        if(tabla_clientes[client]==""){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Cliente inexistente", "El cliente " +client+ " no se encuentra registrado en la base de datos\n"
                                                                         "Desea crear un nuevo cliente con ese nombre?",QMessageBox::Yes|QMessageBox::No);
            if(reply == QMessageBox::Yes) {
                // Get new regional ID
                QString regional_ID = syncCreateRegional ();
                syncCreateClient(client, regional_ID);
            }
            else {
                ui->cliente->setText("");
                ui->guardar_butt->setEnabled(true);
            }
        }
        else {
            QString regional_ID = syncCreateRegional ();
            append_region(regional_ID);
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Seleccionar un cliente y sucursal por favor");
    }
}

void New_client::create_regional(){
    QString city  =  tabla_ciudades[ui -> regional_2 ->text()];
    QString category = tabla_categorias[ui -> categoria ->currentText()];
    QString agent = tabla_agentes[ui -> agente ->currentText()];
    QString anniversary =  ui -> aniversario -> text();

    QJsonArray contacto;
    QJsonObject main_object;
    QJsonDocument reg_doc;

    main_object.insert("city",city);
    main_object.insert("salesAgent",agent);
    main_object.insert("category",category);
    main_object.insert("contacts",contacto);

    if(anniversary!=""){
        main_object.insert("anniversary", QDateTime::fromString(anniversary,"dd/MM/yyyy").toMSecsSinceEpoch());
    }

   reg_doc.setObject(main_object);

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
           QJsonDocument receiver = QJsonDocument::fromJson(binReply);
           append_region(receiver.object().value("regional_client").toObject().value("_id").toString());
       }
       reply->deleteLater ();
   });

   QNetworkRequest request;
   request.setUrl (QUrl ("http://"+this -> url + "/regional_clients"));
   request.setRawHeader ("token", this -> token.toUtf8 ());
   request.setRawHeader ("Content-Type", "application/json");

   nam->post (request, reg_doc.toJson ());
}

void New_client::append_region(QString id_regional){

    QJsonDocument document;

    //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);
    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
        QByteArray binReply = reply->readAll ();
        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
            if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                QMessageBox::critical (this, "Error", QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
               QMessageBox::critical (this, "Error en base de datos", "Reportar este error porfavor" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{

            emit send_update();
            this -> close();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url +"/clients/append_regional?regional="+id_regional+"&client="+tabla_clientes[ui -> cliente -> text()]));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Synchronous creator

void New_client::syncCreateClient(QString clientName, QString regionalID) {

    qDebug() << "regional id =" << regionalID;

    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;
    QJsonArray regionals_array;

    regionals_array.append(regionalID);

    main_object.insert("name", clientName);
    main_object.insert("regionals",regionals_array);
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
            ui -> cliente -> setText(receiver.object().value("client").toObject().value("name").toString());
            set_client();

            emit send_update();
            this -> close();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/clients"));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson ());
}

QString New_client::syncCreateRegional() {
    QString returnValue = "";

    QString city  =  tabla_ciudades[ui -> regional_2 ->text()];
    QString category = tabla_categorias[ui -> categoria ->currentText()];
    QString agent = tabla_agentes[ui -> agente ->currentText()];
    QString anniversary =  ui -> aniversario -> text();

    QJsonArray contacto;
    QJsonObject main_object;
    QJsonDocument reg_doc;

    main_object.insert("city",city);
    main_object.insert("salesAgent",agent);
    main_object.insert("category",category);
    main_object.insert("contacts",contacto);

    if(anniversary!=""){
        main_object.insert("anniversary", QDateTime::fromString(anniversary,"dd/MM/yyyy").toMSecsSinceEpoch());
    }

    reg_doc.setObject(main_object);

    //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/regional_clients"));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    QNetworkReply *reply = nam->post (request, reg_doc.toJson ());

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    /////////////////////////////////////////////

    QByteArray resBin = reply->readAll();
    QJsonDocument response = QJsonDocument::fromJson(resBin);
    if(reply->error()) {
        if (response.object ().value ("err").toObject ().contains ("message")) {
           QMessageBox::critical (this, "Error", QString::fromLatin1 (response.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
        } else {
           QMessageBox::critical (this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta ventana.\n" + QString::fromStdString (response.toJson ().toStdString ()));
        }
    }
    else {
        returnValue = response.object().value("regional_client").toObject().value("_id").toString();
    }

    reply->deleteLater();
    nam->deleteLater();

    return returnValue;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
