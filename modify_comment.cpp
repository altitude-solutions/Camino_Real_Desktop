#include "modify_comment.h"
#include "ui_modify_comment.h"
#include <QScreen>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QJsonDocument>
#include <QDateTime>
#include <QCompleter>
#include <QMessageBox>

Modify_comment::Modify_comment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modify_comment)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

   //Set Widget not resizable
   this->setMinimumWidth(width);
   this->setMinimumHeight(height);

}

Modify_comment::~Modify_comment(){
    delete ui;
}

void Modify_comment::receive_comment(QString token, QString url, QString registro, QString comentario){
    this -> token = token;
    this -> url = url;
    this -> id_register = registro;
    ui -> commentario -> setPlainText(comentario);
}

void Modify_comment::on_pushButton_clicked(){
    //create a Json
    QJsonDocument document;
    QJsonObject main_object;


    if(ui->commentario->toPlainText()!="") {
        main_object.insert("comment",ui->commentario->toPlainText());
    }

    document.setObject(main_object);

    //Send information
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);
    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
        QByteArray binReply = reply->readAll ();
        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (binReply);
            if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                send_info("x","Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
            } else {
                send_info("x", "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
            }
        }
        else{
            send_info("x","Base de datos","Datos actualizados con éxito");
        }
        reply->deleteLater ();
        emit update();
        this ->close();
    });

    QNetworkRequest request;
    request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+this->id_register));
    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");

    nam->put (request, document.toJson ());
}

void Modify_comment::on_pushButton_2_clicked(){
    this -> close();
}


