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
#include <QFile>

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
   ui -> frame_1 -> setFixedWidth(static_cast<int>(width*0.17));

   //set widget size dynamic, aspect ratio 683:384
   double size_w = (width*0.4);
   double size_h = (height)/2.5;
   QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
   this->setFixedSize(size);

   //  Set icons
  double pix_w_a = (width*200)/1366;
  double pix_h_a = (height*200)/768;

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
                    information_box("x","Error","No hay conexión a red");
                }
                else{
                    if (errorJson.object ().value ("err").toObject ().contains ("message")) {
                        information_box("x","Error",QString::fromLatin1 (errorJson.object ().value ("err").toObject ().value ("message").toString ().toLatin1 ()));
                     }
                    else {
                        information_box("x","Error en base de datos","Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
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
    ui -> user -> setText("");
    ui -> password -> setText("");
    main_window.close();
    this->show();
}

void Login::read_url(){

    QString path = QDir::homePath();

    QDir any;
    any.mkdir(path+"/Camino_Real");

    QFile file(path+"/Camino_Real/url.txt");

    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
        }
    }

    if (line == ""){
        this -> url = "69.55.49.12:3000";
//        this -> url = "157.245.241.97:3000";
        //this -> url = "192.168.0.9:3000";

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


void Login::information_box(QString icon, QString header, QString text){

    box_info = new Information_box(this);
    connect(this, SIGNAL(send_info_box(QString, QString, QString, double, double)),box_info, SLOT(receive_info(QString,QString, QString, double, double)));

    //Get screen Size
   const auto screens = qApp->screens();

    int width = screens[0]->geometry().width();
    int height = screens[0]->geometry().height();

     //set widget size dynamic, aspect ratio 16:9
    double w = static_cast<int>(width*0.4);
    double h = static_cast<int>(height)/2.5;

    emit send_info_box(icon, header, text, w, h);
    box_info->show();

}
