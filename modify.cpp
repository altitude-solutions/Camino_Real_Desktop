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

    //Frames size
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/2.5));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.9));

    ui -> frame_2 -> setFixedHeight(static_cast<int>(height*0.05));
    ui -> frame_6 -> setFixedHeight(static_cast<int>(height*0.05));

}

Modify::~Modify()
{
    delete ui;
}

void Modify::on_pushButton_clicked()
{
    this -> close();
}

void Modify::receive_contact(QHash<QString,QString> send_up, QString token, QString url){
    this -> token = token;
    this -> data = send_up;
    this -> url = url;

    ui -> cliente -> setText(data["client"]);
    ui -> regional -> setText(data["regional"]);
    ui -> nombre -> setText(data["contact"]);
    ui -> telefono -> setText(data["phone"]);
    ui -> mail -> setText(data["email"]);
    ui -> cargo -> setText(data["job"]);

}

void Modify::on_tarifario_butt_clicked()
{
    //create a Json
    QJsonDocument document;
    QJsonObject main_object;
    QJsonArray phone;
    QJsonArray mails;
    if(ui->telefono->text()!=""){
        phone.append(ui->telefono->text());
    }
    if(ui->mail->text()!=""){
        mails.append(ui->mail->text());
    }

    main_object.insert("name",ui->nombre->text());
    main_object.insert("phoneNumbers",phone);
    main_object.insert("emailAddresses", mails);
    main_object.insert("job", ui->cargo->text());

    document.setObject(main_object);

    //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);
    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
        QByteArray binReply = reply->readAll ();
       qDebug()<<binReply;
        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
            if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                information_box("x","Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
                //QMessageBox::critical (this, "Error", QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
                information_box("x", "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
                //QMessageBox::critical (this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            //restart();
            emit send_update();
            this -> close();
        }
        reply->deleteLater ();
    });


    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/contacts/"+data["id_contacto"]));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());

}

void Modify::information_box(QString icon, QString header, QString text){

    box_info = new Information_box(this);
    connect(this, SIGNAL(send_info_box(QString, QString, QString, double, double)),box_info, SLOT(receive_info(QString,QString, QString, double, double)));
    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double w = (static_cast<int>(width));
    double h = (static_cast<int>(height*0.9));

    emit send_info_box(icon, header, text, w, h);
    box_info->show();

}
