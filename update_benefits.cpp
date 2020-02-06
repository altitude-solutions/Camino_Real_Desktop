#include "update_benefits.h"
#include "ui_update_benefits.h"
#include <QScreen>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCompleter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

Update_benefits::Update_benefits(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Update_benefits)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

   double size_w = (width);
   double size_h = (height*0.9);
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this -> setMinimumSize(size);

}

Update_benefits::~Update_benefits(){
    delete ui;
}

void Update_benefits::receive_info(QString userName, QString token, QString url, QString register_id, QString early, QString late, QString upgrade, QString noshow){

    this -> userName = userName;
    this -> register_id = register_id;
    this -> token = token;
    this -> url = url;
    this -> early = early;
    this -> late = late;
    this -> upgrade = upgrade;
    this -> noshow = noshow;

    ui -> early ->setText(this -> early);
    ui -> late ->setText(this -> late);
    ui -> upgrade ->setText(this -> upgrade);
    ui -> noshow ->setText(this -> noshow);
}

void Update_benefits::on_cancel_butt_clicked(){
    this -> close();
}

void Update_benefits::on_guardar_butt_clicked(){
    //create a Json
    QJsonDocument document;
    QJsonObject main_object;

    main_object.insert("earlyCheckIn",ui -> early -> text().toInt());
    main_object.insert("lateCheckOut",ui -> late -> text().toInt());
    main_object.insert("upgrade",ui -> upgrade -> text().toInt());
    main_object.insert("noShow",ui -> noshow -> text().toInt());

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
            //Send update to the tab
            send_update();
            send_info_box("benefits","Base de Datos", "Actualización exitosa");
            this -> close();
        }
        reply->deleteLater ();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+this->register_id));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());
}
