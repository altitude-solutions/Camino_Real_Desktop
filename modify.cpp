#include "modify.h"
#include "ui_modify.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QCompleter>

Modify::Modify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modify)
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

  QPixmap pix_cargo(":/images/images/cargo.png");
  ui->icon_regional->setPixmap(pix_cargo.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_regional->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_telefono(":/images/images/telefono.png");
  ui->icon_categoria->setPixmap(pix_telefono.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_categoria->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  //set frame sizes
  ui -> frame -> setFixedWidth(static_cast<int>(width*0.35));
  ui -> frame_7 -> setFixedWidth(static_cast<int>(width*0.35));
}

Modify::~Modify()
{
    delete ui;
}

void Modify::on_cancel_butt_clicked(){

    this -> close();
}

void Modify::receive_contact(QHash<QString, QString>send_up,QString token, QString url){

    this -> id_contacto = send_up["id_contacto"];
    this -> token = token;
    this -> url = url;

    ui -> client -> setText(send_up["client"]);
    ui -> regional -> setText(send_up["regional"]);
    ui -> category -> setText(send_up["category"]);

    ui -> name -> setText(send_up["contact"]);
    ui -> phone -> setText(send_up["phone"]);
    ui -> mail -> setText(send_up["email"]);
    ui -> job -> setText(send_up["job"]);

}

void Modify::on_guardar_butt_clicked(){
    //create a Json
     QJsonDocument document;
     QJsonObject main_object;
     QJsonArray phone;
     QJsonArray mails;

     QString name = ui -> name -> text();
     QString job = ui -> job -> text();

     if(ui->phone->text()!=""){
         phone.append(ui->phone->text());
     }
     if(ui->mail->text()!=""){
         mails.append(ui->mail->text());
     }
     if (ui->name->text()==""){
         name = "-";
     }
     if (job == ""){
         job="-";
     }

     main_object.insert("name",name);
     main_object.insert("phoneNumbers",phone);
     main_object.insert("emailAddresses", mails);
     main_object.insert("job", job);

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
             restart();
             emit send_update();
             this -> close();
         }
         reply->deleteLater ();
     });


     QNetworkRequest request;
     request.setUrl (QUrl ("http://"+this -> url + "/contacts/"+this->id_contacto));
     request.setRawHeader ("token", this -> token.toUtf8 ());
     request.setRawHeader ("Content-Type", "application/json");

     nam->put (request, document.toJson ());

}

void Modify::restart(){

    this -> id_contacto = "";
    this -> token = "";
    this -> url = "";

    ui -> client -> setText("");
    ui -> regional -> setText("");
    ui -> category -> setText("");

    ui -> name -> setText("");
    ui -> phone -> setText("");
    ui -> mail -> setText("");
    ui -> job -> setText("");
}
