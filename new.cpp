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
    set_category();
}

void New::set_completer(){
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

void New::set_clients(){
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
                tabla_vinculo[clientes.toObject().value("name").toString()][regionales.toObject().value("city").toObject().value("city").toString()] = regionales.toObject().value("_id").toString();
                vinculo_categorias[clientes.toObject().value("name").toString()][regionales.toObject().value("city").toObject().value("city").toString()] = regionales.toObject().value("category").toObject().value("category").toString();
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

void New::set_category(){
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
        QCompleter *client_completer = new QCompleter(completer_list,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        ui -> categoria -> setCompleter(client_completer);
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/categories"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void New::set_agents(){
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

        ui -> agent -> setCompleter(client_completer);
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
            ui -> cliente -> setText("");
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Cliente inexistente", "El cliente " +client+ " no se encuentra registrado en la base de datos\n"
                                                                         "Desea crear un nuevo cliente con ese nombre?",QMessageBox::Yes|QMessageBox::No);
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
            this -> city_flag = city;
            ui -> sucursal -> setText("");
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
            if(vinculo_categorias[ui->cliente->text()][city]!=""){
                ui -> categoria -> setText(vinculo_categorias[ui->cliente->text()][city]);
            }
            else{
                ui -> categoria -> setText("");
            }
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
                QMessageBox::critical(this, "Error en base de datos", "Reportar este error porfavor" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            QJsonDocument receiver = QJsonDocument::fromJson(binReply);
            this -> id_ciudad = receiver.object().value("city").toObject().value("city").toString();
            set_completer();
            ui -> sucursal -> setText(this -> city_flag);
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/cities"));
    request.setRawHeader ("token", this -> token.toUtf8());
    request.setRawHeader ("Content-Type", "application/json");

    nam->post (request, document.toJson ());
}

void New::on_categoria_editingFinished()
{
    QString categoria = ui -> categoria -> text();
    if(categoria!=""){
        if(tabla_categorias[categoria]!=""){
            this -> id_categoria = tabla_categorias[categoria];
        }
        else{
            QMessageBox::critical(this,"Error","Categoría no se encuentra registrada en la base de datos");
            ui -> categoria -> setText("");
        }
    }
}

void New::on_agent_editingFinished()
{
    QString agente = ui -> agent -> text();
    if(agente!=""){
        if(tabla_agentes[agente]!=""){
            this -> id_agentes = tabla_agentes[agente];
        }
        else{
            QMessageBox::critical(this,"Error","Usuario no registrado en  base de datos");
            ui -> agent -> setText("");
        }
    }
}

//Sending to database
void New::on_tarifario_butt_clicked(){

    //client info
    QString cliente = ui -> cliente -> text();
    QString ciudad = ui -> sucursal -> text();
    QString categoria = ui -> categoria -> text();

    //Contacto info
    QString name = ui -> nombre -> text();
    QString phone = ui -> telefono -> text();
    QString mail = ui -> mail -> text();
    QString job = ui -> cargo -> text();

    if(cliente!="" && ciudad !="" && categoria!=""){

        QHashIterator<QString, QHash<QString, QString>>iter_regionals(tabla_vinculo);

        this -> append_sucursal = false;

        while (iter_regionals.hasNext()) {
            auto city_key = iter_regionals.next().key();

            if(city_key==ciudad){
                this -> append_sucursal = true;
                this -> id_regional = tabla_vinculo[cliente][city_key];
                break;
            }
        }
        create_contacto();
    }
}

void New::create_contacto(){
    QString nombre_contacto = ui -> nombre -> text();
    QString telefono_contacto = ui -> telefono -> text();
    QString mail_contacto = ui -> mail -> text();
    QString job_contacto = ui -> cargo -> text();

    QJsonArray mails;
    QJsonArray telefonos;
    QJsonObject main_object;
    QJsonDocument contact_doc;

    if(telefono_contacto!=""){
        telefonos.append(telefono_contacto);
        main_object.insert("phoneNumbers",telefonos);
    }
    if(mail_contacto!=""){
        mails.append(mail_contacto);
        main_object.insert("emailAddresses",mails);
    }

    if(nombre_contacto!=""){
        main_object.insert("name",nombre_contacto);
    }
    if(job_contacto!=""){
        main_object.insert("job",job_contacto);
    }

    contact_doc.setObject(main_object);

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
             QJsonDocument receiver = QJsonDocument::fromJson(binReply);
             this -> id_contacto = receiver.object().value("contact").toObject().value("_id").toString();
             qDebug()<<"contacto creado con exito";
             if(this -> append_sucursal){
                    append_to_sucursal();
             }
             else{
                   create_sucursal();
             }
         }
         reply->deleteLater ();
     });

     QNetworkRequest request;
     request.setUrl (QUrl ("http://"+this -> url + "/contacts"));
     request.setRawHeader ("token", this -> token.toUtf8 ());
     request.setRawHeader ("Content-Type", "application/json");

     nam->post (request, contact_doc.toJson ());
}

void New::append_to_sucursal()
{
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
            qDebug()<<"append to sucursal exitoso";
            append_to_client();
        }
        reply->deleteLater ();
    });


    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/regional_clients/"
                                                  "append_contact?contact="+this->id_contacto+"&regional="+this->id_regional));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());
}

void New::create_sucursal(){

    QString send_city = this -> id_ciudad;
    QString send_category = this -> id_categoria;
    QString send_agent = this -> id_agentes;

    QJsonArray contacto;
    QJsonObject main_object;
    QJsonDocument reg_doc;

    contacto.append(this -> id_contacto);

    main_object.insert("city",send_city);
    main_object.insert("salesAgent",send_agent);
    main_object.insert("category",send_category);
    main_object.insert("contacts",contacto);

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
           qDebug()<<"sucursal creada con exito";
           QJsonDocument receiver = QJsonDocument::fromJson(binReply);
           this -> id_regional = receiver.object().value("regional_client").toObject().value("_id").toString();
            append_to_client();
       }
       reply->deleteLater ();
   });

   QNetworkRequest request;
   request.setUrl (QUrl ("http://"+this -> url + "/regional_clients"));
   request.setRawHeader ("token", this -> token.toUtf8 ());
   request.setRawHeader ("Content-Type", "application/json");

   nam->post (request, reg_doc.toJson ());
}

void New::append_to_client(){

    qDebug()<<this -> id_cliente;
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
            qDebug()<<"append a cliente exitoso";
            emit send_update();
            restart();
            this -> close();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url +"/clients/append_regional?regional="+this -> id_regional+"&client="+this -> id_cliente));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());
}

void New::restart(){
    this -> id_ciudad = "";
    this -> id_categoria = "";
    this -> id_cliente = "";
    this -> id_agentes = "";

    ui -> cliente -> setText("");
    ui -> sucursal -> setText("");
    ui -> categoria -> setText("");

    ui -> nombre -> setText("");
    ui -> telefono -> setText("");
    ui -> mail -> setText("");
    ui -> cargo -> setText("");
}
