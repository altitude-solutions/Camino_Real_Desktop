#include "page_2.h"
#include "ui_page_2.h"
#include <QScreen>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCompleter>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

page_2::page_2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_2)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width)/1.8;
    double size_h = (height)/1.6;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

    //  Set Images
   double pix_w_b = (width*40)/1366;
   double pix_h_b= (height*40)/768;

   QPixmap pix_contacto(":/images/images/cliente.png");
   ui->icon_contacto->setPixmap(pix_contacto.scaled( static_cast<int>(pix_w_b*0.7),static_cast<int>(pix_h_b*0.7), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_contacto->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_cargo(":/images/images/cargo.png");
  ui->icon_cargo->setPixmap(pix_cargo.scaled( static_cast<int>(pix_w_b*0.7),static_cast<int>(pix_h_b*0.7), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_cargo->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_telefono(":/images/images/telefono.png");
  ui->icon_telefono->setPixmap(pix_telefono.scaled( static_cast<int>(pix_w_b*0.7),static_cast<int>(pix_h_b*0.7), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_telefono->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

  QPixmap pix_email(":/images/images/e-mail.png");
  ui->icon_email->setPixmap(pix_email.scaled( static_cast<int>(pix_w_b*0.7),static_cast<int>(pix_h_b*0.7), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->icon_email->setFixedSize(static_cast<int>(pix_w_b), static_cast<int>(pix_h_b));

   //  Set icons
  double pix_w_a = (width*43)/1366;
  double pix_h_a = (height*43)/768;

   QPixmap pix_search(":/images/images/lupa-fondo.png");
   QIcon ButtonIcon(pix_search.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_search->setIcon(ButtonIcon);
   ui->icon_search->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

   QPixmap pix_pluss(":/images/images/lapiz-fondo.png");
   QIcon ButtonIcon_2(pix_pluss.scaled( static_cast<int>(pix_w_a),static_cast<int>(pix_h_a), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   ui->icon_pluss->setIcon(ButtonIcon_2);
   ui->icon_pluss->setIconSize(QSize(static_cast<int>(pix_w_a),static_cast<int>(pix_h_a)));

   //Set frame sizes
   ui -> frame_7 -> setFixedWidth(static_cast<int>(width*0.13));
   ui -> frame_8 -> setFixedWidth(static_cast<int>(width*0.13));
   ui -> frame_7 -> setFixedHeight(static_cast<int>(height*0.08));
   ui -> frame_8 -> setFixedHeight(static_cast<int>(height*0.08));
   ui -> frame_10 -> setFixedHeight(static_cast<int>(height*0.08));
   ui -> frame_12 -> setFixedHeight(static_cast<int>(height*0.08));

   ui -> frame_2 -> setFixedWidth(static_cast<int>(width*0.13));
   ui -> frame_3 -> setFixedWidth(static_cast<int>(width*0.13));
   ui -> frame_9 -> setFixedWidth(static_cast<int>(width*0.13));
   ui -> frame_4 -> setFixedWidth(static_cast<int>(width*0.13));
}

page_2::~page_2()
{
    delete ui;
}

void page_2::on_pushButton_15_clicked()
{
    emit send_info_back();
}

//This section is for "VIA"
void page_2::on_email_butt_clicked()
{
    via = "E-mail";
    paint_via(via);
}

void page_2::on_visita_butt_clicked()
{
    via = "Visita";
    paint_via(via);
}

void page_2::on_telefono_butt_clicked()
{
    via = "Telefono";
    paint_via(via);
}

void page_2::on_wpp_butt_clicked()
{
    via = "Whatsapp";
    paint_via(via);
}

//This section is for "MOTIVO"
void page_2::on_seguimiento_butt_clicked()
{
    motivo = "Seguimiento";
    paint_motivo(motivo);
}

void page_2::on_fidelizacion_butt_clicked()
{
    motivo = "Fidelizacion";
    paint_motivo(motivo);
}

void page_2::on_prospeccion_butt_clicked()
{
    motivo = "Prospeccion";
    paint_motivo(motivo);
}

void page_2::on_bajomov_butt_clicked()
{
    motivo = "Bajo Movimiento";
    paint_motivo(motivo);
}

void page_2::on_primer_butt_clicked()
{
    motivo = "Primer Contacto";
    paint_motivo(motivo);
}

//This section is for "RESULTADO"
void page_2::on_tarifario_butt_clicked()
{
    resultado = "Tarifario";
    paint_resultado(resultado);
}

void page_2::on_cotizacion_butt_clicked()
{
    resultado = "Cotizacion";
    paint_resultado(resultado);
}

void page_2::on_reserva_butt_clicked()
{
    resultado = "Reserva";
    paint_resultado(resultado);

    //send the signal Back
    emit send_nights_back();
}

void page_2::on_no_interesa_clicked()
{
    resultado = "No le Interesa";
    paint_resultado(resultado);
}


void page_2::paint_via(QString painter){
    QString released = "font: 10pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,50%);"
                                    "min-width:8em;"
                                    "max-width:8em;"
                                    "min-height:2em;"
                                    "max-height:2em;";

    QString pushed = "font: 10pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8em;"
                                       "max-width:8em;"
                                       "min-height:2em;"
                                       "max-height:2em;";

    if (painter ==  "E-mail"){
        ui -> email_butt -> setStyleSheet(pushed);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
    }
    else if(painter == "Visita"){
        ui -> email_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(pushed);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
    }
    else if(painter == "Telefono"){
        ui -> email_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(pushed);
        ui -> wpp_butt -> setStyleSheet(released);
    }
    else if (painter == "Whatsapp"){
        ui -> email_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(pushed);
    }
}

void page_2::paint_motivo(QString painter){
    QString released = "font: 10pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,50%);"
                                    "min-width:9em;"
                                    "max-width:9em;"
                                    "min-height:2em;"
                                    "max-height:2em;";

    QString pushed = "font: 10pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:9em;"
                                       "max-width:9em;"
                                       "min-height:2em;"
                                       "max-height:2em;";

    if (painter ==  "Seguimiento"){
        ui -> seguimiento_butt -> setStyleSheet(pushed);
        ui -> fidelizacion_butt -> setStyleSheet(released);
        ui -> prospeccion_butt -> setStyleSheet(released);
        ui -> bajomov_butt -> setStyleSheet(released);
        ui -> primer_butt -> setStyleSheet(released);
    }
    else if(painter == "Fidelizacion"){
        ui -> seguimiento_butt -> setStyleSheet(released);
        ui -> fidelizacion_butt -> setStyleSheet(pushed);
        ui -> prospeccion_butt -> setStyleSheet(released);
        ui -> bajomov_butt -> setStyleSheet(released);
        ui -> primer_butt -> setStyleSheet(released);
    }
    else if(painter == "Prospeccion"){
        ui -> seguimiento_butt -> setStyleSheet(released);
        ui -> fidelizacion_butt -> setStyleSheet(released);
        ui -> prospeccion_butt -> setStyleSheet(pushed);
        ui -> bajomov_butt -> setStyleSheet(released);
        ui -> primer_butt -> setStyleSheet(released);
    }
    else if (painter == "Bajo Movimiento"){
        ui -> seguimiento_butt -> setStyleSheet(released);
        ui -> fidelizacion_butt -> setStyleSheet(released);
        ui -> prospeccion_butt -> setStyleSheet(released);
        ui -> bajomov_butt -> setStyleSheet(pushed);
        ui -> primer_butt -> setStyleSheet(released);
    }
    else if(painter == "Primer Contacto"){
        ui -> seguimiento_butt -> setStyleSheet(released);
        ui -> fidelizacion_butt -> setStyleSheet(released);
        ui -> prospeccion_butt -> setStyleSheet(released);
        ui -> bajomov_butt -> setStyleSheet(released);
        ui -> primer_butt -> setStyleSheet(pushed);
    }
}

void page_2::paint_resultado(QString painter){
    QString released = "font: 10pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,50%);"
                                    "min-width:8em;"
                                    "max-width:8em;"
                                    "min-height:2em;"
                                    "max-height:2em;";

    QString pushed = "font: 10pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8em;"
                                       "max-width:8em;"
                                       "min-height:2em;"
                                       "max-height:2em;";

    if (painter ==  "Tarifario"){
        ui -> tarifario_butt -> setStyleSheet(pushed);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> no_interesa -> setStyleSheet(released);
    }
    else if(painter == "Cotizacion"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(pushed);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> no_interesa -> setStyleSheet(released);
    }
    else if(painter == "Nueva Cotizacion"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> no_interesa -> setStyleSheet(released);
    }
    else if (painter == "Reserva"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(pushed);
        ui -> no_interesa -> setStyleSheet(released);
    }
    else if(painter == "No le Interesa"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> no_interesa -> setStyleSheet(pushed);
    }
}

void page_2::receiver(QString userName, QString realName, QString token, QString url){
    this -> userName = userName;
    this -> realName = realName;
    this -> token = token;
    this -> url = url;

    read_client_info();
}

void page_2::receive_nights(QString nights){
      this -> n_nights = nights;
}

void page_2::receive_extra_info(QString competencia, QString tarifa, QString noches){
    this -> competencia = competencia;
    this -> tarifa = tarifa;
    this -> noches = noches;
}

void page_2::read_client_info()
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
                    db_clients.insert(entidad.toObject ().value("name").toString()+"//"+regional.toObject ().value("city").toString(), current);

                    //Información de contacto
                    contactos[contacto.toObject ().value ("_id").toString()]["name"] = contacto.toObject ().value ("name").toString();
                    contactos[contacto.toObject ().value ("_id").toString()]["phone"] = contacto.toObject ().value ("phoneNumbers").toArray().at(0).toString();
                    contactos[contacto.toObject ().value ("_id").toString()]["email"] = contacto.toObject ().value ("emailAddresses").toArray().at(0).toString();
                    contactos[contacto.toObject ().value ("_id").toString()]["job"] = contacto.toObject ().value ("job").toString();
                    contactos[contacto.toObject ().value ("_id").toString()]["regional"] = regional.toObject ().value("_id").toString();
                    contactos[contacto.toObject ().value ("_id").toString()]["client"] = entidad.toObject ().value("_id").toString();

                    clientes[entidad.toObject ().value("_id").toString()]["name"] = entidad.toObject ().value("name").toString();
                    clientes[entidad.toObject ().value("_id").toString()][regional.toObject ().value("_id").toString()] =regional.toObject ().value("city").toString();

                    regionales[regional.toObject ().value("_id").toString()] = regional.toObject ().value("city").toString();
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

void page_2::on_cliente_editingFinished()
{
    QString local = ui -> cliente -> text();
    if(local!=""){

        QStringList client_info = local.split("//");

        if (client_info.size()>1){
            QString client = client_info[0];
            QString region = client_info[1];

             QHashIterator<QString, QHash<QString,QString>>iter_contact(contactos);
             QHashIterator<QString, QHash<QString,QString>>iter_client(clientes);
             QHashIterator<QString, QString>iter_region(regionales);
             QString client_id = "";
             QString regional_id = "";
             QString auxiliar = "";

             ui -> contacto_drop -> clear();
             contact_data.clear();

            while (iter_client.hasNext()) {
                auto client_key = iter_client.next().key();
                if(client == clientes[client_key]["name"]){
                     client_id =   client_key;

                     while (iter_region.hasNext()) {
                         auto region_key = iter_region.next().key();
                          if(region==regionales[region_key] && clientes[client_id][region_key]!=""){
                              regional_id = region_key;
                          }
                     }
                     break;
                }
            }

           if (client_id !="" && regional_id!=""){
                 while(iter_contact.hasNext()){

                     auto contact_key = iter_contact.next().key();

                     if (client_id == contactos[contact_key]["client"] && regional_id == contactos[contact_key]["regional"]) {

                         ui -> contacto_drop -> addItem(contactos[contact_key]["name"]);
                         ui -> contacto_drop -> setCurrentText(contactos[contact_key]["name"]);

                         contact_data[contact_key] = contactos[contact_key]["name"];

                         ui -> cargo -> setText(contactos[contact_key]["job"]);
                         ui -> telefono -> setText(contactos[contact_key]["phone"]);
                         ui -> mail -> setText(contactos[contact_key]["email"]);

                         contact_name = contactos[contact_key]["name"];
                         contact_phone = contactos[contact_key]["phone"];
                         contact_mail = contactos[contact_key]["email"];
                         contact_job = contactos[contact_key]["job"];
                         cid = contact_key;
                     }
                 }
            }
        }
    }
}


void page_2::on_contacto_drop_currentTextChanged(const QString &arg1)
{
    QHashIterator<QString,QString>iter_contact(contact_data);

    if(arg1!=""){
        while (iter_contact.hasNext()) {
             auto main_key = iter_contact.next().key();
             if(arg1 == contact_data[main_key]){

                 ui -> cargo -> setText(contactos[main_key]["job"]);
                 ui -> telefono -> setText(contactos[main_key]["phone"]);
                 ui -> mail -> setText(contactos[main_key]["email"]);

                 contact_name = contactos[main_key]["name"];
                 contact_phone = contactos[main_key]["phone"];
                 contact_mail = contactos[main_key]["email"];
                 contact_job = contactos[main_key]["job"];
                 cid = main_key;
             }
        }
   }
}

void page_2::on_icon_search_clicked()
{
    on_cliente_editingFinished();
}

void page_2::on_pushButton_14_clicked()
{
    //create a Json
    QString hash_id = ui -> cliente -> text();
    QString comments = ui -> comentarios -> toPlainText();
    QJsonDocument document;
    QStringList saved;
    QJsonObject main_object;
    QString time = QDateTime::currentDateTime().toString("dd/MM/yyyy - hh:mm:ss");

    if (via!=""&&motivo!=""&&resultado!=""){

        main_object.insert("client", db_clients[hash_id]["id_cliente"]);
        main_object.insert("regional", db_clients[hash_id]["id_regional"]);
        main_object.insert("contact", cid);
        main_object.insert("via", via);
        main_object.insert("reason", motivo);
        main_object.insert("result", resultado);
        main_object.insert("date", QDateTime::fromString(time,"dd/MM/yyyy - hh:mm:ss").toMSecsSinceEpoch());

        if (comments != ""){
            main_object.insert("comments", comments);
        }
        if ( this-> n_nights!=""){
            main_object.insert("nights", this-> n_nights.toInt());
        }
        if (this -> competencia != ""){
            main_object.insert("competition", this->competencia);
        }
        if (this -> tarifa != ""){
            main_object.insert("budget", this->tarifa);
        }
        if (this -> noches != ""){
            main_object.insert("estimateNights", this->noches.toInt());
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
                information_box("x", "Base de Datos","Guardado con éxito");
                //QMessageBox::information(this, "Base de Datos", "Guardado con éxito");
            }
            reply->deleteLater ();
        });


        QNetworkRequest request;
        request.setUrl (QUrl ("http://"+this -> url + "/out_data"));
        request.setRawHeader ("token", this -> token.toUtf8 ());
        request.setRawHeader ("Content-Type", "application/json");

        nam->post (request, document.toJson ());

    }
    else{
        information_box("x","Datos incompletos ","Seleccionar VÍA, MOTIVO y RESULTADO");
        //QMessageBox::critical (this, "Datos incompletos", "Seleccionar VÍA, MOTIVO y RESULTADO");
    }
}

void page_2::restart(){
    via = "";
    motivo = "";
    resultado = "";

    contact_name="";
    contact_phone="";
    contact_mail="";
    contact_job="";
    cid = "";

    ui -> contacto_drop -> clear();
    ui -> cliente -> setText("");
    ui -> mail -> setText("");
    ui -> telefono -> setText("");
    ui -> cargo -> setText("");
    ui -> comentarios -> setPlainText("");

    this -> n_nights = "";
    this -> competencia = "";
    this -> tarifa = "";
    this -> noches = "";

    QString released_a = "font: 10pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,100%);"
                                    "min-width:8em;"
                                    "max-width:8em;"
                                    "min-height:2em;"
                                    "max-height:2em;";

    ui -> email_butt -> setStyleSheet(released_a);
    ui -> visita_butt -> setStyleSheet(released_a);
    ui -> telefono_butt -> setStyleSheet(released_a);
    ui -> wpp_butt -> setStyleSheet(released_a);

    QString released_b = "font: 10pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,100%);"
                                    "min-width:9em;"
                                    "max-width:9em;"
                                    "min-height:2em;"
                                    "max-height:2em;";

    ui -> seguimiento_butt -> setStyleSheet(released_b);
    ui -> fidelizacion_butt -> setStyleSheet(released_b);
    ui -> prospeccion_butt -> setStyleSheet(released_b);
    ui -> bajomov_butt -> setStyleSheet(released_b);
    ui -> primer_butt -> setStyleSheet(released_b);

    ui -> tarifario_butt -> setStyleSheet(released_a);
    ui -> cotizacion_butt -> setStyleSheet(released_a);
    ui -> reserva_butt -> setStyleSheet(released_a);
    ui -> no_interesa -> setStyleSheet(released_a);
}

void page_2::information_box(QString icon, QString header, QString text){
    emit send_info_box(icon, header, text);
}

void page_2::on_icon_pluss_clicked()
{
    emit send_clients();
}

