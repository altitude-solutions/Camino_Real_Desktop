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

  //set frame sizes
  ui -> frame -> setFixedWidth(static_cast<int>(width*0.4));
  ui -> frame_7 -> setFixedWidth(static_cast<int>(width*0.4));
}

New::~New(){
    delete ui;
}

void New::on_cancel_butt_clicked(){
    this -> close();
}

void New::receive_info(QString token, QString url){
    this -> token = token;
    this -> url = url;

    //Set all requiered information
    set_clients();
    set_city();
}

void New::on_regional_box_currentIndexChanged(const QString &city){
    QString cliente  = ui -> cliente -> currentText();
    ui -> categoria -> setText(vinculo_categorias[cliente][city]);
}

void New::on_cliente_currentIndexChanged(const QString &arg1){
    QString current_client = arg1;
    ui -> regional_box -> clear();
    foreach (QString city, this -> completer_list) {
        if(tabla_vinculo[current_client][city]!=""){
                ui -> regional_box -> addItem(city);
        }
    }
    QString city = ui -> regional_box -> currentText();
    ui -> categoria -> setText(vinculo_categorias[current_client][city]);
}

void New::set_city(){
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            QMessageBox::critical(this,"Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);

        foreach (QJsonValue ciudades, okJson.object ().value ("cities").toArray ()) {
            tabla_ciudades[ciudades.toObject().value("city").toString()] = ciudades.toObject().value("_id").toString();
            this -> completer_list << ciudades.toObject().value("city").toString();
          }
        this -> completer_list.removeDuplicates();
        std::sort(this -> completer_list.begin(), this -> completer_list.end());

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
        foreach (QString item, completer_list) {
            ui -> cliente -> addItem(item);
        }
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/clients"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void New::on_guardar_butt_clicked()
{
    QString cliente = ui -> cliente -> currentText();
    QString ciudad = ui -> regional_box -> currentText();

    this -> id_regional = tabla_vinculo[cliente][ciudad];

    ui -> guardar_butt -> setDisabled(true);

    create_contact();
}

void New::create_contact(){
    QString nombre_contacto = ui -> name  -> text();
    QString telefono_contacto = ui -> phone-> text();
    QString mail_contacto = ui -> mail -> text();
    QString job_contacto = ui -> job -> text();

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
             append_contacto();
         }
         reply->deleteLater ();
     });

     QNetworkRequest request;
     request.setUrl (QUrl ("http://"+this -> url + "/contacts"));
     request.setRawHeader ("token", this -> token.toUtf8 ());
     request.setRawHeader ("Content-Type", "application/json");

     nam->post (request, contact_doc.toJson ());
}

void New::append_contacto(){

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
    request.setUrl (QUrl ("http://"+this -> url + "/regional_clients/"
                                                  "append_contact?contact="+this->id_contacto+"&regional="+this->id_regional));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());
}



