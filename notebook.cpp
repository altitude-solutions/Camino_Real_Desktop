#include "notebook.h"
#include "ui_notebook.h"
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

Notebook::Notebook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notebook)
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


    //Frames size:
    ui -> frame_4 -> setFixedWidth(static_cast<int>(width/1.45));
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.5));

    //Set the table Size
    ui -> your_table -> setColumnCount(6);
    ui->your_table ->setColumnWidth(0,static_cast<int>(width/11)); //Fecha
    ui->your_table ->setColumnWidth(1,static_cast<int>(width/13));  //Cliente
    ui->your_table ->setColumnWidth(2,static_cast<int>(width/18));  //Regional
    ui->your_table ->setColumnWidth(3,static_cast<int>(width/18));  //Task
    ui->your_table ->setColumnWidth(4,static_cast<int>(width/2.5));  //Comentarios
    ui->your_table ->setColumnWidth(5,0);

    //Setting the table headers
    QStringList headers = {"Fecha",
                           "Cliente",
                           "Regional",
                           "Task",
                           "Comentarios"};

    ui -> your_table -> setHorizontalHeaderLabels(headers);

    //Set the table Size
    ui -> general_table -> setColumnCount(7);
    ui->general_table ->setColumnWidth(0,static_cast<int>(width/11)); //Fecha
    ui->general_table ->setColumnWidth(1,static_cast<int>(width/12));  //Cliente
    ui->general_table ->setColumnWidth(2,static_cast<int>(width/18));  //Regional
    ui->general_table ->setColumnWidth(3,static_cast<int>(width/18));  //Task
    ui->general_table ->setColumnWidth(4,static_cast<int>(width/3.2));  //Comentarios
    ui->general_table ->setColumnWidth(5,static_cast<int>(width/12));  //Asesor
    ui->general_table ->setColumnWidth(6,0);

    //Setting the table headers
    QStringList headers_b = {"Fecha",
                           "Cliente",
                           "Regional",
                           "Task",
                           "Comentarios",
                           "Asesor"};

    ui -> general_table -> setHorizontalHeaderLabels(headers_b);

}

Notebook::~Notebook()
{
    delete ui;
}

void Notebook::information_box(QString icon, QString header, QString text)
{
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

void Notebook::receive_info(QString userName, QString realName, QString token, QString url)
{
    this -> userName = userName;
    this -> realName = realName;
    this -> token   = token;
    this -> url = url;

    //Initialize filters
    this -> your_client_filter = "";
    this -> your_regional_filter = "";

    this -> gral_client_filter = "";
    this -> gral_regional_filter = "";
    this -> gral_assistant_filter = "";

    all_gral_info();
    all_your_info();
    read_your_info(your_client_filter, your_regional_filter);
    read_gral_info(gral_client_filter, gral_regional_filter, gral_assistant_filter);

}

void Notebook::read_your_info(QString a, QString b){
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            information_box("x","Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        this -> your_filtered_tasks.clear();

        foreach (QJsonValue registro, okJson.object ().value ("tasks").toArray ()) {

            QHash<QString, QString> current;
            current.insert ("id_register", registro.toObject ().value ("_id").toString ());
            current.insert ("date", QDateTime::fromMSecsSinceEpoch(registro.toObject ().value ("registerDate").toVariant().toLongLong()).toString("dd/MM/yyyy - hh:mm"));
            current.insert ("task", registro.toObject ().value ("todo").toString());
            current.insert("comments",registro.toObject ().value ("comment").toString());

            //Client data
            current.insert("id_client",registro.toObject ().value("client").toObject().value("_id").toString());
            current.insert("client",registro.toObject ().value("client").toObject().value("name").toString());

            //Regional data
            current.insert("id_regional",registro.toObject ().value("regional").toObject().value("_id").toString());
            current.insert("regional",registro.toObject ().value("regional").toObject().value("city").toString());

            //Agent data
            current.insert("id_agent",registro.toObject ().value("creationAgent").toObject().value("_id").toString());
            current.insert("agent",registro.toObject ().value("creationAgent").toObject().value("realName").toString());

            if(registro.toObject ().value("creationAgent").toObject().value("realName").toString() == this->realName){
                your_filtered_tasks.insert(registro.toObject ().value("_id").toString(), current);
            }
         }

        update_your_table(your_filtered_tasks);
        reply->deleteLater ();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/tasks?deleted=0&completed=0"+a+b));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get(request);
}

void Notebook::read_gral_info(QString a, QString b, QString c){
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            information_box("x","Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        this -> gral_filtered_tasks.clear();

        foreach (QJsonValue registro, okJson.object ().value ("tasks").toArray ()) {

            QHash<QString, QString> current;
            current.insert ("id_register", registro.toObject ().value ("_id").toString ());
            current.insert ("date", QDateTime::fromMSecsSinceEpoch(registro.toObject ().value ("registerDate").toVariant().toLongLong()).toString("dd/MM/yyyy - hh:mm"));
            current.insert ("task", registro.toObject ().value ("todo").toString());
            current.insert("comments",registro.toObject ().value ("comment").toString());

            //Client data
            current.insert("id_client",registro.toObject ().value("client").toObject().value("_id").toString());
            current.insert("client",registro.toObject ().value("client").toObject().value("name").toString());

            //Regional data
            current.insert("id_regional",registro.toObject ().value("regional").toObject().value("_id").toString());
            current.insert("regional",registro.toObject ().value("regional").toObject().value("city").toString());

            //Agent data
            current.insert("id_agent",registro.toObject ().value("creationAgent").toObject().value("_id").toString());
            current.insert("agent",registro.toObject ().value("creationAgent").toObject().value("realName").toString());

            if(registro.toObject ().value("creationAgent").toObject().value("realName").toString() != this->realName){
                gral_filtered_tasks.insert(registro.toObject ().value("_id").toString(), current);
            }
         }

        update_gral_table(gral_filtered_tasks);
        reply->deleteLater ();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/tasks?deleted=0&completed=0"+a+b+c));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get(request);
}

void Notebook::update_your_table(QHash<QString, QHash<QString, QString>>update){

    //Rewrite the local table
    ui -> your_table -> setRowCount(0);

    QHashIterator<QString, QHash<QString, QString>>iter(update);
    ui->your_table->setSortingEnabled(false);
    while(iter.hasNext()){

        auto current = iter.next().key();

        //Add a new row
        int  row_control;
        ui->your_table->insertRow(ui->your_table->rowCount());
        row_control= ui->your_table->rowCount()-1;

        //Writing the current row
        ui->your_table->setItem(row_control, 0, new QTableWidgetItem(update[current]["date"]));
        ui->your_table->setItem(row_control, 1, new QTableWidgetItem(update[current]["client"]));
        ui->your_table->setItem(row_control, 2, new QTableWidgetItem(update[current]["regional"]));
        ui->your_table->setItem(row_control, 3, new QTableWidgetItem(update[current]["task"]));
        ui->your_table->setItem(row_control, 4, new QTableWidgetItem(update[current]["comments"]));
        ui->your_table->setItem(row_control, 5, new QTableWidgetItem(current));

        ui->your_table->item(row_control, 0 ) -> setTextAlignment(Qt::AlignCenter);
        ui->your_table->item(row_control, 1 ) -> setTextAlignment(Qt::AlignCenter);
        ui->your_table->item(row_control, 2) -> setTextAlignment(Qt::AlignCenter);
        ui->your_table->item(row_control, 4) -> setTextAlignment(Qt::AlignCenter);
        ui->your_table->item(row_control, 3) -> setTextAlignment(Qt::AlignCenter);
        ui->your_table->item(row_control, 5 ) -> setTextAlignment(Qt::AlignCenter);
    }

    ui -> your_table -> setSortingEnabled(true);
}

void Notebook::on_your_table_cellClicked(int row, int column)
{
    qDebug()<<column;

    your_reg["date"] = ui -> your_table -> item(row,0)->text();
    your_reg["client"] = ui -> your_table -> item(row,1)->text();
    your_reg["regional"] = ui -> your_table -> item(row,2)->text();
    your_reg["task"] = ui -> your_table -> item(row,3)->text();
    your_reg["comments"] = ui -> your_table -> item(row,4)->text();
    your_reg["id_register"] = ui -> your_table -> item(row,5)->text();

    paint_your_table(row);
}

void Notebook::paint_your_table(int row){
    for (int i=0; i<your_filtered_tasks.size(); i++){
        if (i==row){
            ui->your_table->item(row,0)->setBackground(QColor("#B6B6B6"));
            ui->your_table->item(row,1)->setBackground(QColor("#B6B6B6"));
            ui->your_table->item(row,2)->setBackground(QColor("#B6B6B6"));
            ui->your_table->item(row,3)->setBackground(QColor("#B6B6B6"));
            ui->your_table->item(row,4)->setBackground(QColor("#B6B6B6"));
            ui->your_table->item(row,5)->setBackground(QColor("#B6B6B6"));
        }
        else {
            ui->your_table->item(i,0)->setBackground(QColor("#FFFFFF"));
            ui->your_table->item(i,1)->setBackground(QColor("#FFFFFF"));
            ui->your_table->item(i,2)->setBackground(QColor("#FFFFFF"));
            ui->your_table->item(i,3)->setBackground(QColor("#FFFFFF"));
            ui->your_table->item(i,4)->setBackground(QColor("#FFFFFF"));
            ui->your_table->item(i,5)->setBackground(QColor("#FFFFFF"));
        }
    }
}


void Notebook::update_gral_table(QHash<QString, QHash<QString, QString>> update){
    //Rewrite the local table
    ui -> general_table -> setRowCount(0);

    QHashIterator<QString, QHash<QString, QString>>iter(update);
    ui->general_table->setSortingEnabled(false);
    while(iter.hasNext()){

        auto current = iter.next().key();

        //Add a new row
        int  row_control;
        ui->general_table->insertRow(ui->general_table->rowCount());
        row_control= ui->general_table->rowCount()-1;

        //Writing the current row
        ui->general_table->setItem(row_control, 0, new QTableWidgetItem(update[current]["date"]));
        ui->general_table->setItem(row_control, 1, new QTableWidgetItem(update[current]["client"]));
        ui->general_table->setItem(row_control, 2, new QTableWidgetItem(update[current]["regional"]));
        ui->general_table->setItem(row_control, 3, new QTableWidgetItem(update[current]["task"]));
        ui->general_table->setItem(row_control, 4, new QTableWidgetItem(update[current]["comments"]));
        ui->general_table->setItem(row_control, 5, new QTableWidgetItem(update[current]["agent"]));
        ui->general_table->setItem(row_control, 6, new QTableWidgetItem(current));

        ui->general_table->item(row_control, 0 ) -> setTextAlignment(Qt::AlignCenter);
        ui->general_table->item(row_control, 1 ) -> setTextAlignment(Qt::AlignCenter);
        ui->general_table->item(row_control, 2) -> setTextAlignment(Qt::AlignCenter);
        ui->general_table->item(row_control, 3) -> setTextAlignment(Qt::AlignCenter);
        ui->general_table->item(row_control, 4) -> setTextAlignment(Qt::AlignCenter);
        ui->general_table->item(row_control, 5 ) -> setTextAlignment(Qt::AlignCenter);
        ui->general_table->item(row_control, 6 ) -> setTextAlignment(Qt::AlignCenter);

    }
    ui -> general_table -> setSortingEnabled(true);
}

void Notebook::on_general_table_cellClicked(int row, int column)
{
    qDebug()<<column;

    gral_reg["date"] = ui -> general_table -> item(row,0)->text();
    gral_reg["client"] = ui -> general_table -> item(row,1)->text();
    gral_reg["regional"] = ui -> general_table -> item(row,2)->text();
    gral_reg["task"] = ui -> general_table -> item(row,3)->text();
    gral_reg["comments"] = ui -> general_table -> item(row,4)->text();
    gral_reg["agent"] = ui -> general_table -> item(row,5)->text();
    gral_reg["id_register"] = ui -> general_table -> item(row,6)->text();

    paint_gral_table(row);
}

void Notebook::paint_gral_table(int row){

    for (int i=0; i<gral_filtered_tasks.size(); i++){
        if (i==row){
            ui->general_table->item(row,0)->setBackground(QColor("#B6B6B6"));
            ui->general_table->item(row,1)->setBackground(QColor("#B6B6B6"));
            ui->general_table->item(row,2)->setBackground(QColor("#B6B6B6"));
            ui->general_table->item(row,3)->setBackground(QColor("#B6B6B6"));
            ui->general_table->item(row,4)->setBackground(QColor("#B6B6B6"));
            ui->general_table->item(row,5)->setBackground(QColor("#B6B6B6"));
        }
        else {
            ui->general_table->item(i,0)->setBackground(QColor("#FFFFFF"));
            ui->general_table->item(i,1)->setBackground(QColor("#FFFFFF"));
            ui->general_table->item(i,2)->setBackground(QColor("#FFFFFF"));
            ui->general_table->item(i,3)->setBackground(QColor("#FFFFFF"));
            ui->general_table->item(i,4)->setBackground(QColor("#FFFFFF"));
            ui->general_table->item(i,5)->setBackground(QColor("#FFFFFF"));
        }
    }
}

void Notebook::restart()
{
    //Initialize filters
    this -> your_client_filter = "";
    this -> your_regional_filter = "";

    this -> gral_client_filter = "";
    this -> gral_regional_filter = "";
    this -> gral_assistant_filter = "";

    all_gral_info();
    all_your_info();
    read_your_info(your_client_filter, your_regional_filter);
    read_gral_info(gral_client_filter, gral_regional_filter, gral_assistant_filter);
}

void Notebook::all_your_info()
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
        this -> all_your_tasks.clear();
        QStringList client_list;
        QStringList regional_list;

        foreach (QJsonValue registro, okJson.object ().value ("tasks").toArray ()) {

            QHash<QString, QString> current;
            current.insert ("id_register", registro.toObject ().value ("_id").toString ());
            current.insert ("date", QDateTime::fromMSecsSinceEpoch(registro.toObject ().value ("registerDate").toVariant().toLongLong()).toString());
            current.insert ("task", registro.toObject ().value ("todo").toString());
            current.insert("comments",registro.toObject ().value ("comment").toString());

            //Client data
            current.insert("id_client",registro.toObject ().value("client").toObject().value("_id").toString());
            current.insert("client",registro.toObject ().value("client").toObject().value("name").toString());
            client_list<<registro.toObject ().value("client").toObject().value("name").toString();

            //Regional data
            current.insert("id_regional",registro.toObject ().value("regional").toObject().value("_id").toString());
            current.insert("regional",registro.toObject ().value("regional").toObject().value("city").toString());
            regional_list<<registro.toObject ().value("regional").toObject().value("city").toString();

            //Agent data
            current.insert("id_agent",registro.toObject ().value("creationAgent").toObject().value("_id").toString());
            current.insert("agent",registro.toObject ().value("creationAgent").toObject().value("realName").toString());

            if(registro.toObject ().value("creationAgent").toObject().value("realName").toString() == this->realName){
                all_your_tasks.insert(registro.toObject ().value("_id").toString(), current);
            }
         }

        std::sort(client_list.begin(), client_list.end());
        client_list.removeDuplicates();

        std::sort(regional_list.begin(), regional_list.end());
        regional_list.removeDuplicates();

        QCompleter *client_completer = new QCompleter(client_list,this);
        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        QCompleter *regional_completer = new QCompleter(regional_list,this);
        regional_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        regional_completer -> setCompletionMode(QCompleter::PopupCompletion);
        regional_completer -> setFilterMode(Qt::MatchContains);

        ui -> cliente -> setCompleter(client_completer);
        ui -> regional -> setCompleter(regional_completer);

        reply->deleteLater ();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/tasks?deleted=0&completed=0"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get(request);
}

void Notebook::all_gral_info()
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
        this -> all_gral_tasks.clear();
        QStringList client_list;
        QStringList regional_list;
        QStringList agent_list;

        foreach (QJsonValue registro, okJson.object ().value ("tasks").toArray ()) {

            QHash<QString, QString> current;
            current.insert ("id_register", registro.toObject ().value ("_id").toString ());
            current.insert ("date", QDateTime::fromMSecsSinceEpoch(registro.toObject ().value ("registerDate").toVariant().toLongLong()).toString());
            current.insert ("task", registro.toObject ().value ("todo").toString());
            current.insert("comments",registro.toObject ().value ("comment").toString());

            //Client data
            current.insert("id_client",registro.toObject ().value("client").toObject().value("_id").toString());
            current.insert("client",registro.toObject ().value("client").toObject().value("name").toString());
            client_list<<registro.toObject ().value("client").toObject().value("name").toString();

            //Regional data
            current.insert("id_regional",registro.toObject ().value("regional").toObject().value("_id").toString());
            current.insert("regional",registro.toObject ().value("regional").toObject().value("city").toString());
            regional_list<<registro.toObject ().value("regional").toObject().value("city").toString();

            //Agent data
            current.insert("id_agent",registro.toObject ().value("creationAgent").toObject().value("_id").toString());
            current.insert("agent",registro.toObject ().value("creationAgent").toObject().value("realName").toString());
            agent_list<<registro.toObject ().value("creationAgent").toObject().value("realName").toString();

            if(registro.toObject ().value("creationAgent").toObject().value("realName").toString() != this->realName){
                all_gral_tasks.insert(registro.toObject ().value("_id").toString(), current);
            }
         }

        std::sort(client_list.begin(), client_list.end());
        client_list.removeDuplicates();

        std::sort(regional_list.begin(), regional_list.end());
        regional_list.removeDuplicates();

        std::sort(agent_list.begin(), agent_list.end());
        agent_list.removeDuplicates();

        QCompleter *client_completer = new QCompleter(client_list,this);
        client_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        client_completer -> setCompletionMode(QCompleter::PopupCompletion);
        client_completer -> setFilterMode(Qt::MatchContains);

        QCompleter *regional_completer = new QCompleter(regional_list,this);
        regional_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        regional_completer -> setCompletionMode(QCompleter::PopupCompletion);
        regional_completer -> setFilterMode(Qt::MatchContains);

        QCompleter *agent_completer = new QCompleter(agent_list,this);
        agent_completer -> setCaseSensitivity(Qt::CaseInsensitive);
        agent_completer -> setCompletionMode(QCompleter::PopupCompletion);
        agent_completer -> setFilterMode(Qt::MatchContains);

        ui -> cliente_2 -> setCompleter(client_completer);
        ui -> regional_2 -> setCompleter(regional_completer);
        ui -> asesor_2 -> setCompleter(agent_completer);

        reply->deleteLater ();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/tasks?deleted=0&completed=0"));

    request.setRawHeader ("token", this -> token.toUtf8 ());
    request.setRawHeader ("Content-Type", "application/json");
    nam->get(request);
}

//Your tasks
void Notebook::on_cliente_editingFinished()
{
    QString filtered =  ui -> cliente -> text();

    QHashIterator<QString, QHash<QString,QString>>iter(all_your_tasks);
    QString find = "no";
    QString id;

    if(filtered!=""){
        while (iter.hasNext()){
            auto key = iter.next().key();
            if(filtered==all_your_tasks[key]["client"]){
                find = "yes";
                id = all_your_tasks[key]["id_client"];
            }
        }
        if(find!="no"){
            this -> your_client_filter = "&client="+id;
            read_your_info(this->your_client_filter, this->your_regional_filter);
        }
        else{
            information_box("x","Registro inexistente","No existen pendientes con ese cliente");
            ui -> cliente -> setText("");
        }
    }
    else{
        this -> your_client_filter = "";
        read_your_info(this->your_client_filter, this->your_regional_filter);
    }
}

void Notebook::on_regional_editingFinished()
{
    QString filtered =  ui -> regional -> text();

    QHashIterator<QString, QHash<QString,QString>>iter(all_your_tasks);
    QString find = "no";
    QString id;

    if(filtered!=""){
        while (iter.hasNext()){
            auto key = iter.next().key();
            if(filtered==all_your_tasks[key]["regional"]){
                find = "yes";
                id = all_your_tasks[key]["id_regional"];
            }
        }
        if(find!="no"){
            this -> your_regional_filter = "&region="+id;
            read_your_info(this->your_client_filter, this->your_regional_filter);
        }
        else{
            information_box("x","Registro inexistente","No existen pendientes con ese cliente");
            ui -> regional -> setText("");
        }
    }
    else{
        this -> your_regional_filter = "";
        read_your_info(this->your_client_filter, this->your_regional_filter);
    }
}

//All gral Tasks
void Notebook::on_cliente_2_editingFinished()
{
    QString filtered =  ui -> cliente_2 -> text();

    QHashIterator<QString, QHash<QString,QString>>iter(all_gral_tasks);
    QString find = "no";
    QString id;

    if(filtered!=""){
        while (iter.hasNext()){
            auto key = iter.next().key();
            if(filtered==all_gral_tasks[key]["client"]){
                find = "yes";
                id = all_gral_tasks[key]["id_client"];
            }
        }
        if(find!="no"){
            this -> gral_client_filter = "&client="+id;
            read_gral_info(this->gral_client_filter, this->gral_regional_filter, this -> gral_assistant_filter);
        }
        else{
            information_box("x","Registro inexistente","No existen pendientes con ese cliente");
            ui -> cliente_2 -> setText("");
        }
    }
    else{
        this -> gral_client_filter = "";
        read_gral_info(this->gral_client_filter, this->gral_regional_filter, this -> gral_assistant_filter);
    }
}

void Notebook::on_regional_2_editingFinished()
{
    QString filtered =  ui -> regional_2 -> text();

    QHashIterator<QString, QHash<QString,QString>>iter(all_gral_tasks);
    QString find = "no";
    QString id;

    if(filtered!=""){
        while (iter.hasNext()){
            auto key = iter.next().key();
            if(filtered==all_gral_tasks[key]["regional"]){
                find = "yes";
                id = all_gral_tasks[key]["id_regional"];
            }
        }
        if(find!="no"){
            this -> gral_regional_filter = "&region="+id;
            read_gral_info(this->gral_client_filter, this->gral_regional_filter, this -> gral_assistant_filter);
        }
        else{
            information_box("x","Registro inexistente","No existen pendientes con ese cliente");
            ui -> regional_2 -> setText("");
        }
    }
    else{
        this -> gral_regional_filter = "";
        read_gral_info(this->gral_client_filter, this->gral_regional_filter, this -> gral_assistant_filter);
    }
}

void Notebook::on_asesor_2_editingFinished()
{
    QString filtered =  ui -> asesor_2 -> text();

    QHashIterator<QString, QHash<QString,QString>>iter(all_gral_tasks);
    QString find = "no";
    QString id;

    if(filtered!=""){
        while (iter.hasNext()){
            auto key = iter.next().key();
            if(filtered==all_gral_tasks[key]["agent"]){
                find = "yes";
                id = all_gral_tasks[key]["id_agent"];
            }
        }
        if(find!="no"){
            this -> gral_assistant_filter = "&agent="+id;
            read_gral_info(this->gral_client_filter, this->gral_regional_filter, this -> gral_assistant_filter);
        }
        else{
            information_box("x","Registro inexistente","No existen pendientes con ese cliente");
            ui -> asesor_2 -> setText("");
        }
    }
    else{
        this -> gral_assistant_filter = "";
        read_gral_info(this->gral_client_filter, this->gral_regional_filter, this -> gral_assistant_filter);
    }
}

void Notebook::on_delete_butt_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Terminar task", "Seguro desea dar por terminado este task?",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(your_reg["task"]!=""){
            //create a Json
            QJsonDocument document;
            QJsonObject main_object;
            QString tiempo = QDateTime::currentDateTime().toString("dd/MM/yyyy")+" - "+QDateTime::currentDateTime().toString("hh:mm:ss");

            main_object.insert("completed",true);
            main_object.insert("doneDate",QDateTime::fromString(tiempo,"dd/MM/yyyy - hh:mm:ss").toMSecsSinceEpoch());

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
                }
                reply->deleteLater ();
            });

            QNetworkRequest request;
            request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+your_reg["id_register"]));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");

            nam->put (request, document.toJson ());
        }
    }
}

void Notebook::on_delete_butt_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Terminar task", "Seguro desea dar por terminado este task?",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(gral_reg["task"]!=""){
            //create a Json
            QJsonDocument document;
            QJsonObject main_object;
            QString tiempo = QDateTime::currentDateTime().toString("dd/MM/yyyy")+" - "+QDateTime::currentDateTime().toString("hh:mm:ss");

            main_object.insert("completed",true);
            main_object.insert("doneDate",QDateTime::fromString(tiempo,"dd/MM/yyyy - hh:mm:ss").toMSecsSinceEpoch());

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
                }
                reply->deleteLater ();
            });

            QNetworkRequest request;
            request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+gral_reg["id_register"]));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");

            nam->put (request, document.toJson ());
        }
    }
}

void Notebook::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Terminar task", "Seguro desea eliminar este task?",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(your_reg["id_register"]!=""){
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
                restart();
                reply->deleteLater ();
            });

            QNetworkRequest request;
            request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+your_reg["id_register"]));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");

            nam->sendCustomRequest(request,"DELETE");
        }
        else{
            information_box("x","Seleccionar registro","Porfavor indicar el registro que desea eliminar");
        }
    }
}

void Notebook::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Terminar task", "Seguro desea eliminar este task?",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(gral_reg["id_register"]!=""){
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
                restart();
                reply->deleteLater ();
            });

            QNetworkRequest request;
            request.setUrl (QUrl ("http://"+this -> url + "/tasks/"+gral_reg["id_register"]));
            request.setRawHeader ("token", this -> token.toUtf8 ());
            request.setRawHeader ("Content-Type", "application/json");

            nam->sendCustomRequest(request,"DELETE");
        }
        else{
            information_box("x","Seleccionar registro","Porfavor indicar el registro que desea eliminar");
        }
    }
}
