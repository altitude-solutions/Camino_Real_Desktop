#include "page_1.h"
#include "ui_page_1.h"
#include <QScreen>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCompleter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

page_1::page_1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_1)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width)/2;
    double size_h = (height)/1.8;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

    //  Set icons
   double pix_w_b = (width*111)/1920;
   double pix_h_b= (height*60)/1080;

   QPixmap pix_client(":/images/images/cliente-fondo.png");
   ui->icon_cliente->setPixmap(pix_client.scaled( static_cast<int>(pix_w_b),static_cast<int>(pix_h_b), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_cliente->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

   //  Set icons
  double pix_w_a = (width*43)/1920;
  double pix_h_a = (height*43)/1080;

   QPixmap pix_search(":/images/images/lupa-fondo.png");
   QIcon ButtonIcon(pix_search.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_search->setIcon(ButtonIcon);
   ui->icon_search->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

   QPixmap pix_pluss(":/images/images/lapiz-fondo.png");
   QIcon ButtonIcon_2(pix_pluss.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_pluss->setIcon(ButtonIcon_2);
   ui->icon_pluss->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));
}

page_1::~page_1()
{
    delete ui;
}


//This section is for "VÍA"
void page_1::on_mail_butt_clicked()
{
    via = "E-mail";
    paint_via(via);
}

void page_1::on_visita_butt_clicked()
{
    via = "Visita";
    paint_via(via);
}

void page_1::on_telefono_butt_clicked()
{
    via = "Telefono";
    paint_via(via);
}

void page_1::on_wpp_butt_clicked()
{
    via = "Whatsapp";
    paint_via(via);
}

void page_1::paint_via(QString painter){

    QString released = "font: 12pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,50%);"
                                    "min-width:8.5em;"
                                    "max-width:8.5em;"
                                    "min-height:2.5em;"
                                    "max-height:2.5em;";

    QString pushed = "font: 12pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8.5em;"
                                       "max-width:8.5em;"
                                       "min-height:2.5em;"
                                       "max-height:2.5em;";

    if (painter ==  "E-mail"){
        ui -> mail_butt -> setStyleSheet(pushed);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
    }
    else if(painter == "Visita"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(pushed);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
    }
    else if(painter == "Telefono"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(pushed);
        ui -> wpp_butt -> setStyleSheet(released);
    }
    else if (painter == "Whatsapp"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(pushed);
    }
}


//This section is for "MOTIVO"
void page_1::on_tarifario_butt_clicked()
{
    motivo = "Tarifario";
    paint_motivo(motivo);
}

void page_1::on_cotizacion_butt_clicked()
{
    motivo = "Cotizacion";
    paint_motivo(motivo);
}

void page_1::on_reserva_butt_clicked()
{
    motivo = "Reserva";
    paint_motivo(motivo);

    nights = new Nights(this);
    connect(this, SIGNAL(send_page(QString)),nights, SLOT(receive_page(QString)));
    connect (nights, &Nights::send_nights, this, &page_1::receive_nights);
    emit send_page("in_page");
    nights->show();
}

void page_1::on_otros_butt_clicked()
{
    motivo = "Otros";
    paint_motivo(motivo);
}

void page_1::paint_motivo(QString painter){
    QString released = "font: 12pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,50%);"
                                    "min-width:8.5em;"
                                    "max-width:8.5em;"
                                    "min-height:2.5em;"
                                    "max-height:2.5em;";

    QString pushed = "font: 12pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8.5em;"
                                       "max-width:8.5em;"
                                       "min-height:2.5em;"
                                       "max-height:2.5em;";

    if (painter ==  "Tarifario"){
        ui -> tarifario_butt -> setStyleSheet(pushed);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(released);
    }
    else if(painter == "Cotizacion"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(pushed);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(released);
    }
    else if(painter == "Reserva"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(pushed);
        ui -> otros_butt -> setStyleSheet(released);
    }
    else if (painter == "Otros"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(pushed);
    }
}

void page_1::receiver(QString userName, QString realName, QString token, QString url){
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    via = "";
    motivo = "";
    read_client_info();
}

void page_1::receive_nights(QString nights){
      this -> n_nights = nights;
    qDebug()<<"page_1:  "<<this->n_nights;
}

void page_1::read_client_info()
{
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            information_box("x","Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            //QMessageBox::critical (this, "Error", QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        foreach (QJsonValue entidad, okJson.object ().value ("clients").toArray ()) {
            foreach (QJsonValue regional, entidad.toObject().value("regionals").toArray()) {
                foreach( QJsonValue contacto, regional.toObject().value("contacts").toArray()){

                    QHash<QString, QString> current;
                    current.insert ("contact", contacto.toObject ().value ("name").toString ());
                    current.insert ("job", contacto.toObject ().value ("job").toString());
                    current.insert ("phone", contacto.toObject ().value ("phoneNumbers").toArray().at(0).toString());
                    current.insert ("email", contacto.toObject ().value ("emailAddresses").toArray().at(0).toString());
                    current.insert ("id_contacto", contacto.toObject ().value ("_id").toString());

                    current.insert("regional",regional.toObject ().value("city").toString());
                    current.insert("id_regional",regional.toObject ().value("_id").toString());
                    current.insert("category",regional.toObject ().value("category").toString());

                    current.insert("agent_id",regional.toObject ().value("salesAgent").toObject().value("_id").toString());
                    current.insert("user_name",regional.toObject ().value("salesAgent").toObject().value("userName").toString());
                    current.insert("real_name",regional.toObject ().value("salesAgent").toObject().value("realName").toString());

                    current.insert("id_cliente",entidad.toObject ().value("_id").toString());
                    db_clients.insert(entidad.toObject ().value("name").toString()+"/"+regional.toObject ().value("city").toString(), current);
                }
             }
          }
        //Extracting labels for routes
        QHashIterator<QString, QHash<QString, QString>>client_iter(db_clients);
        QStringList client_list;

        while(client_iter.hasNext()){
            client_list<<client_iter.next().key();
        }
        std::sort(client_list.begin(), client_list.end());
        QCompleter *client_completer = new QCompleter(client_list,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        ui -> cliente -> setCompleter(client_completer);
        reply->deleteLater ();

    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/clients?from=0&to=1000&status=1"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get (request);
}

void page_1::on_cliente_editingFinished()
{
   QString client = ui -> cliente -> text();

   if(client!=""){
        ui -> contacto -> setText(db_clients[client]["contact"]);
        ui -> cargo -> setText(db_clients[client]["job"]);
        ui -> telefono -> setText(db_clients[client]["phone"]);
        ui -> mail -> setText(db_clients[client]["email"]);
   }
}

void page_1::on_icon_search_clicked()
{
    on_cliente_editingFinished();
}

void page_1::on_pushButton_9_clicked()
{

    //create a Json
    QString hash_id = ui -> cliente -> text();
    QString comments = ui -> comentarios -> toPlainText();
    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;
    QString time = QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm:ss");

    if (via!=""&&motivo!="" ){
        main_object.insert("client", db_clients[hash_id]["id_cliente"]);
        main_object.insert("regional", db_clients[hash_id]["id_regional"]);
        main_object.insert("contact", db_clients[hash_id]["id_contacto"]);
        main_object.insert("via", via);
        main_object.insert("reason", motivo);        
        main_object.insert("date", QDateTime::fromString(time,"dd/MM/yyyy - hh:mm:ss").toMSecsSinceEpoch());

        if (comments!=""){
            main_object.insert("comments", comments);
        }

        if (this -> n_nights != ""){
            main_object.insert("nights", this -> n_nights.toInt());
        }
        document.setObject(main_object);

        //Send information
        QNetworkAccessManager* nam = new QNetworkAccessManager (this);
        connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {
            QByteArray binReply = reply->readAll ();
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
                restart();
                information_box("x", "Base de Datos", "Guardado con éxito");

                //QMessageBox::information(this, "Base de Datos", "Guardado con éxito");
            }
            reply->deleteLater ();
        });


        QNetworkRequest request;
        request.setUrl (QUrl ("http://"+this -> url + "/in_data"));
        request.setRawHeader ("token", this -> token.toUtf8 ());
        request.setRawHeader ("Content-Type", "application/json");

        nam->post (request, document.toJson ());

    }
    else{
        information_box("x", "Datos incompletos", "Seleccionar VÍA y MOTIVO");
        //QMessageBox::critical (this, "Datos incompletos", "Seleccionar VÍA y MOTIVO");
    }
}

void page_1::restart(){
    via = "";
    motivo = "";

    ui -> cliente -> setText("");
    ui -> contacto -> setText("");
    ui -> mail -> setText("");
    ui -> telefono -> setText("");
    ui -> cargo -> setText("");
    ui -> comentarios -> setPlainText("");

    this -> n_nights  = "";

    QString pushed = "font: 12pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8.5em;"
                                       "max-width:8.5em;"
                                       "min-height:2.5em;"
                                       "max-height:2.5em;";

    ui -> tarifario_butt -> setStyleSheet(pushed);
    ui -> cotizacion_butt -> setStyleSheet(pushed);
    ui -> reserva_butt -> setStyleSheet(pushed);
    ui -> otros_butt -> setStyleSheet(pushed);

    ui -> mail_butt -> setStyleSheet(pushed);
    ui -> visita_butt -> setStyleSheet(pushed);
    ui -> telefono_butt -> setStyleSheet(pushed);
    ui -> wpp_butt -> setStyleSheet(pushed);
}

void page_1::information_box(QString icon, QString header, QString text){

    box_info = new Information_box(this);
    connect(this, SIGNAL(send_info_box(QString, QString, QString, double, double)),box_info, SLOT(receive_info(QString,QString, QString, double, double)));

    //Get screen Size
   const auto screens = qApp->screens();

    int width = screens[0]->geometry().width();
    int height = screens[0]->geometry().height();

     //set widget size dynamic, aspect ratio 16:9
     double w = (width)/2;
     double h = (height)/1.8;

    emit send_info_box(icon, header, text, w, h);
    box_info->show();

}

void page_1::on_icon_pluss_clicked()
{
    emit send_clients();
}
