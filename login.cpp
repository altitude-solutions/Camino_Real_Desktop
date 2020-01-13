#include "login.h"
#include "ui_login.h"
#include <QScreen>
#include <QPixmap>
#include <QDesktopWidget>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDir>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    main_window.hide();

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //frames sized
   ui -> frame_1 -> setFixedWidth(static_cast<int>(width*0.15));

   //set widget size dynamic, aspect ratio 16:9
   double size_w = (width)/3;
   double size_h = (height)/3;
   QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
   this->setFixedSize(size);

   //  Set icons
  double pix_w_a = (width*220)/1920;
  double pix_h_a = (height*220)/1080;

  QPixmap main_pix(":/images/images/camino_real.png");
  ui->main_icon->setPixmap(main_pix.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_w_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->main_icon->setFixedSize(static_cast<int>(pix_w_a), static_cast<int>(pix_h_a));

  //Connects
  connect(&main_window, &MainWindow::logOut,this,&Login::closeSession);
  connect(this, SIGNAL(retreive_data(QString, QString, QString, QString)), &main_window, SLOT(receive_info(QString,QString,QString,QString)));

  read_url();

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    /*************************************************************************
                                             Network Connection
    **************************************************************************/
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);

    //Lambda function
    connect(nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
            QByteArray resBin = reply->readAll ();
            if (reply-> error()){
                QJsonDocument errorJson = QJsonDocument::fromJson( resBin );

                //error catch
                if(reply -> error() == QNetworkReply::UnknownNetworkError){
                    QMessageBox::critical (this, "Error", "No hay conexión");
                }
                else{
                    if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                        QMessageBox::critical (this, "Error", QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
                    }
                    else {
                        QMessageBox::critical (this, "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
                    }
                }
                ui -> pushButton -> setEnabled (true);
            }
            else{
                QJsonDocument response = QJsonDocument::fromJson ( resBin );

                ui -> pushButton->setEnabled (true);
                emit retreive_data (response.object().value("user").toObject().value("userName").toString(), QString::fromLatin1 ( response.object().value("user").toObject().value("realName").toString().toLatin1() ), response.object().value("token").toString(),this->url);
                main_window.show ();
                this->hide ();
            }
            reply -> deleteLater();
        });

    QNetworkRequest req;

    //TODO --> Change to config file
    req.setUrl (QUrl ("http://"+this->url+"/login"));
    req.setRawHeader ("Content-Type", "application/json");

    QJsonDocument body;
    QJsonObject bodyContent;

    bodyContent.insert ("userName", ui -> user ->text ());
    bodyContent.insert ("password", ui -> password->text ());
    body.setObject (bodyContent);

    nam->post(req, body.toJson());

    ui -> pushButton -> setDisabled(true);

}

void Login::closeSession()
{
    main_window.close();
    this->show();
}

void Login::read_url(){

    QString path = QDir::homePath();

    QFile file(path+"/Camino Real/url.txt");

    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
        }
    }

    if (line == ""){
        this -> url = "192.168.0.9:3000";

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream( &file );
            stream<<this->url;
        }

    }
    else{
        this -> url =  line;
    }

    file.close();
}
