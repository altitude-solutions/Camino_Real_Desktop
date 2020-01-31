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
    double size_w = (width)/1.8;
    double size_h = (height)/1.6;
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));
    this->setFixedSize(size);

    //Set frame sizes
    ui -> frame_5 -> setFixedWidth(static_cast<int>(width*0.13));
    ui -> frame_6 -> setFixedWidth(static_cast<int>(width*0.13));
    ui -> frame_5 -> setFixedHeight(static_cast<int>(height*0.08));
    ui -> frame_6 -> setFixedHeight(static_cast<int>(height*0.08));
    ui -> frame_7 -> setFixedHeight(static_cast<int>(height*0.08));
    ui -> frame_8 -> setFixedHeight(static_cast<int>(height*0.08));

    ui -> frame_2 -> setFixedWidth(static_cast<int>(width*0.13));
    ui -> frame_3 -> setFixedWidth(static_cast<int>(width*0.13));
    ui -> frame_9 -> setFixedWidth(static_cast<int>(width*0.13));
    ui -> frame -> setFixedWidth(static_cast<int>(width*0.13));

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


void page_1::on_pushButton_clicked()
{
    via = "Pagina web";
    paint_via(via);
}

void page_1::paint_via(QString painter){

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
        ui -> mail_butt -> setStyleSheet(pushed);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
        ui -> pushButton -> setStyleSheet(released);
    }
    else if(painter == "Visita"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(pushed);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
        ui -> pushButton -> setStyleSheet(released);
    }
    else if(painter == "Telefono"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(pushed);
        ui -> wpp_butt -> setStyleSheet(released);
        ui -> pushButton -> setStyleSheet(released);
    }
    else if (painter == "Whatsapp"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(pushed);
        ui -> pushButton -> setStyleSheet(released);
    }
    else if (painter == "Pagina web"){
        ui -> mail_butt -> setStyleSheet(released);
        ui -> visita_butt -> setStyleSheet(released);
        ui -> telefono_butt -> setStyleSheet(released);
        ui -> wpp_butt -> setStyleSheet(released);
        ui -> pushButton -> setStyleSheet(pushed);
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

    //send the signal Back
    emit send_nights_back();
}

void page_1::on_otros_butt_clicked()
{
    motivo = "Otros";
    paint_motivo(motivo);
}


void page_1::on_reclamo_clicked()
{
    motivo = "Reclamo";
    paint_motivo(motivo);
}


void page_1::paint_motivo(QString painter){
    QString released = "font: 10pt \"MS Shell Dlg 2\";"
                                    "color:white;"
                                    "background-color:rgba(121,99,78,50%);"
                                    "min-width:8.5em;"
                                    "max-width:8.5em;"
                                    "min-height:2em;"
                                    "max-height:2em;";

    QString pushed = "font: 10pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8.5em;"
                                       "max-width:8.5em;"
                                       "min-height:2em;"
                                       "max-height:2em;";

    if (painter ==  "Tarifario"){
        ui -> tarifario_butt -> setStyleSheet(pushed);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(released);
        ui -> reclamo -> setStyleSheet(released);
    }
    else if(painter == "Cotizacion"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(pushed);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(released);
        ui -> reclamo -> setStyleSheet(released);
    }
    else if(painter == "Reserva"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(pushed);
        ui -> otros_butt -> setStyleSheet(released);
        ui -> reclamo -> setStyleSheet(released);
    }
    else if (painter == "Otros"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(pushed);
        ui -> reclamo -> setStyleSheet(released);
    }
    else if (painter == "Reclamo"){
        ui -> tarifario_butt -> setStyleSheet(released);
        ui -> cotizacion_butt -> setStyleSheet(released);
        ui -> reserva_butt -> setStyleSheet(released);
        ui -> otros_butt -> setStyleSheet(released);
        ui -> reclamo -> setStyleSheet(pushed);
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

//Function to receive number of nights
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
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        QStringList completer_list;

        foreach (QJsonValue entidad, okJson.object ().value ("clients").toArray ()) {
            QString cliente = entidad.toObject ().value("name").toString();
            QString id_cliente = entidad.toObject ().value("_id").toString();
            this -> tabla_clientes[cliente] = id_cliente;

            foreach (QJsonValue regional, entidad.toObject().value("regionals").toArray()) {

                QString ciudad = regional.toObject().value("city").toObject().value("city").toString();
                QString id_ciudad = regional.toObject().value("city").toObject().value("_id").toString();
                QString id_sucursal = regional.toObject().value("_id").toString();
                this -> tabla_regionales[id_cliente][ciudad] = id_sucursal;

                QString categoria = regional.toObject().value("category").toObject().value("category").toString();
                QString id_categoria = regional.toObject().value("category").toObject().value("_id").toString();
                this -> tabla_categorias[categoria] = id_categoria;

                QString agente = regional.toObject().value("salesAgent").toObject().value("realName").toString();
                QString id_agente = regional.toObject().value("salesAgent").toObject().value("_id").toString();
                this -> tabla_agentes[agente] = id_agente;

                completer_list << cliente + " / " + ciudad;

                foreach( QJsonValue contacto, regional.toObject().value("contacts").toArray()){

                    QHash<QString,QStringList>current_contact;

                    QString id_contacto = contacto.toObject().value("_id").toString();
                    QString nombre_contacto = contacto.toObject().value("name").toString();
                    QString cargo_contacto = contacto.toObject().value("job").toString();
                    bool default_contact = contacto.toObject().value("primary").toBool();

                    QStringList phones;
                    QStringList emails;

                    foreach (QJsonValue telefono, contacto.toObject().value("phoneNumbers").toArray()) {
                        phones << telefono.toString();
                    }

                    foreach (QJsonValue email, contacto.toObject().value("emailAddresses").toArray()) {
                        emails << email.toString();
                    }

                    this -> tabla_telefonos[id_contacto] = phones;
                    this -> tabla_mails[id_contacto] = emails;

                    this -> tabla_contactos[id_contacto]["contact"] = nombre_contacto;
                    this -> tabla_contactos[id_contacto]["id_contacto"] = id_contacto;
                    this -> tabla_contactos[id_contacto]["job"] = cargo_contacto;
                    this -> tabla_contactos[id_contacto]["client"] = id_cliente;
                    this -> tabla_contactos[id_contacto]["region"] = id_ciudad;
                    this -> tabla_contactos[id_contacto]["category"] = id_categoria;
                    this -> tabla_contactos[id_contacto]["default"] = default_contact;
                    this -> tabla_contactos[id_contacto]["city"] = ciudad;
                }
             }
          }

        completer_list.removeDuplicates();
        std::sort(completer_list.begin(), completer_list.end());
        QCompleter *client_completer = new QCompleter(completer_list,this);

        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        ui -> cliente -> setCompleter(client_completer);
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
    ui -> telefono -> setText("");
    ui -> mail -> setText("");
    ui -> cargo -> setText("");

    QString local = ui -> cliente -> text();
   if(local!=""){

       QStringList client_info = local.split(" / ");
       if (client_info.size()>1){

           QString client = client_info[0];
           QString region = client_info[1];
           QString sucursal;

           client = tabla_clientes[client];
           sucursal = tabla_regionales[client][region];

           this -> client_selected = client;
           this -> regional_selected = sucursal;

           ui -> contacto_drop -> clear();
           contact_data.clear();

           QHashIterator<QString,QHash<QString, QString>>iter_clients(tabla_contactos);

           qDebug()<<"second flag";
           while (iter_clients.hasNext()){

               auto contact_key = iter_clients.next().key();
               if(tabla_contactos[contact_key]["client"] == client && tabla_contactos[contact_key]["city"] == region){

                   ui -> contacto_drop -> addItem(tabla_contactos[contact_key]["contact"]);
                   contact_data[contact_key] = tabla_contactos[contact_key]["contact"];

                   if( tabla_contactos[contact_key]["default"] == true){

                       ui -> contacto_drop -> setCurrentText(tabla_contactos[contact_key]["contact"]);
                       ui -> cargo -> setText(tabla_contactos[contact_key]["job"]);

                       QString telefono;
                       QString mail;

                       qDebug()<<"Crash";
                       if(tabla_telefonos[contact_key].isEmpty()){
                           telefono = "-";
                       }
                       else{
                           telefono = tabla_telefonos[contact_key][0];
                       }


                       if(tabla_mails[contact_key].isEmpty()){
                           mail = "-";
                       }
                       else{
                           mail = tabla_mails[contact_key][0];
                       }

                       ui -> telefono -> setText(telefono);
                       ui -> mail -> setText(mail);
                        qDebug()<<"worked";
                       contact_name = tabla_contactos[contact_key]["contact"];
                       contact_phone = telefono;
                       contact_mail = mail;
                       contact_job = tabla_contactos[contact_key]["job"];
                       cid = contact_key;
                   }
               }
            }
        }
    }
}

void page_1::on_contacto_drop_currentTextChanged(const QString &arg1)
{
      QHashIterator<QString,QString>iter_contact(contact_data);

      if(arg1!=""){
          while (iter_contact.hasNext()) {
               auto main_key = iter_contact.next().key();
               if(arg1 == contact_data[main_key]){

                   QString telefono;
                   QString mail;

                   if(tabla_telefonos[main_key].isEmpty()){
                       telefono = "-";
                   }
                   else{
                       telefono = tabla_telefonos[main_key][0];
                   }


                   if(tabla_mails[main_key].isEmpty()){
                       mail = "-";
                   }
                   else{
                       mail = tabla_mails[main_key][0];
                   }

                   ui -> cargo -> setText(tabla_contactos[main_key]["job"]);
                   ui -> telefono -> setText(telefono);
                   ui -> mail -> setText(mail);

                   contact_name = tabla_contactos[main_key]["contact"];
                   contact_phone = telefono;
                   contact_mail = mail;
                   contact_job = mail;
                   cid = main_key;
               }
          }
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
        main_object.insert("client", this -> client_selected);
        main_object.insert("regional", this -> regional_selected);
        main_object.insert("contact", cid);
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
                } else {
                    information_box("x", "Error en base de datos", "Por favor enviar un reporte de error con una captura de pantalla de esta venta.\n" + QString::fromStdString (errorJson.toJson ().toStdString ()));
                }
            }
            else{
                restart();
                information_box("x", "Base de Datos", "Guardado con éxito");
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
    }
}

void page_1::restart(){
    via = "";
    motivo = "";

    ui -> cliente -> setText("");
    ui -> mail -> setText("");
    ui -> telefono -> setText("");
    ui -> cargo -> setText("");
    ui -> comentarios -> setPlainText("");

    contact_name="";
    contact_phone="";
    contact_mail="";
    contact_job="";
    cid = "";

    this -> client_selected = "";
    this -> regional_selected = "";

    ui -> contacto_drop -> clear();

    contact_data.clear();

    this -> n_nights  = "";

    QString pushed_a = "font: 10pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8em;"
                                       "max-width:8em;"
                                       "min-height:2em;"
                                       "max-height:2em;";

    QString pushed_b = "font: 10pt \"MS Shell Dlg 2\";"
                                       "color:white;"
                                       "background-color:#79634E;"
                                       "min-width:8.5em;"
                                       "max-width:8.5em;"
                                       "min-height:2em;"
                                       "max-height:2em;";

    ui -> tarifario_butt -> setStyleSheet(pushed_a);
    ui -> cotizacion_butt -> setStyleSheet(pushed_a);
    ui -> reserva_butt -> setStyleSheet(pushed_a);
    ui -> otros_butt -> setStyleSheet(pushed_a);
    ui -> pushButton -> setStyleSheet(pushed_b);

    ui -> mail_butt -> setStyleSheet(pushed_b);
    ui -> visita_butt -> setStyleSheet(pushed_b);
    ui -> telefono_butt -> setStyleSheet(pushed_b);
    ui -> wpp_butt -> setStyleSheet(pushed_b);
    ui -> reclamo -> setStyleSheet(pushed_a);
}

void page_1::information_box(QString icon, QString header, QString text){

    //Send information Box back
    emit send_info_box(icon, header, text);

}

void page_1::on_icon_pluss_clicked()
{
    emit send_clients();
}



