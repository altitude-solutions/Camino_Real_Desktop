#include "update_client.h"
#include "ui_update_client.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCompleter>

Update_client::Update_client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Update_client)
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
  ui -> frame -> setFixedWidth(static_cast<int>(width*0.35));
  ui -> frame_6 -> setFixedWidth(static_cast<int>(width*0.35));
  ui -> frame_8 -> setFixedWidth(static_cast<int>(width*0.35));

}

Update_client::~Update_client()
{
    delete ui;
}

void Update_client::receive_contact(QHash<QString,QString> send_up, QString token, QString url){
    this -> token = token;
    this -> data = send_up;
    this -> url = url;

    //Set ui here
    ui -> cliente -> setText(send_up["Client"]);
    ui -> regional -> setText(send_up["City"]);
    ui -> aniversario -> setText(send_up["Anniversary"]);
    ui -> categoria -> setCurrentText(send_up["Category"]);
    ui -> agente -> setCurrentText(send_up["Agent"]);

    this -> id_client = send_up["client_id"];
    this -> id_sucursal = send_up["sucursal_id"];
    this -> id_ciudad = send_up["id_ciudad"];
    this -> id_categoria = send_up["id_categoria"];
    this -> id_agente = send_up["id_agente"];

    //Completer setters
    set_category();
    set_agents();
}

void Update_client::on_cancel_butt_clicked(){
    this -> close();
}

void Update_client::set_category(){
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

void Update_client::set_agents(){
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

        foreach (QString item,  completer_list) {
            ui -> agente -> addItem(item);
        }

    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/users"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void Update_client::on_aniversario_editingFinished(){

    QString anniversary = ui -> aniversario -> text();
    if(anniversary!=""){
        qlonglong time_validator = QDateTime::fromString(anniversary,"dd/MM/yyyy").toMSecsSinceEpoch();
        if(time_validator==14400000){
            QMessageBox::warning(this,"Error","Ingresar un formato de fecha válido");
            ui -> aniversario -> setText("");
        }
    }
}

void Update_client::on_guardar_butt_clicked(){

    //create a Json
     QJsonDocument document;
     QJsonObject main_object;

     if(ui -> agente -> currentText() !=""){
         main_object.insert("salesAgent",tabla_agentes[ui -> agente -> currentText()]);
     }
     if(ui -> categoria -> currentText() !=""){
         main_object.insert("category",tabla_categorias[ui -> categoria->currentText()]);
     }
     if(ui -> aniversario -> text() !=""){
         main_object.insert("anniversary",QDateTime::fromString(ui -> aniversario ->text(),"dd/MM/yyyy").toMSecsSinceEpoch());
     }

     document.setObject(main_object);

     //Send information
     QNetworkAccessManager* nam = new QNetworkAccessManager (this);
     connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
         QByteArray binReply = reply->readAll ();
        qDebug()<<binReply;
         if (reply->error ()) {
             QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
             if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                 QMessageBox::warning(this,"Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
             } else {
                 QMessageBox::warning(this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
               }
         }
         else{
             emit send_update();
             this -> close();
         }
         reply->deleteLater ();
     });

     QNetworkRequest request;
     request.setUrl (QUrl ("http://"+this -> url + "/regional_clients/"+this->id_sucursal));
     request.setRawHeader ("token", this -> token.toUtf8 ());
     request.setRawHeader ("Content-Type", "application/json");

     nam->put (request, document.toJson ());
}
