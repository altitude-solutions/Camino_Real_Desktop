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
    contacto.insert("name",name );
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
                //information_box("x","Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
                QMessageBox::critical (this, "Error", QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
                //information_box("x", "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
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

