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
    ui -> frame_4 -> setFixedHeight(static_cast<int>(height/1.8));

    //Set the table Size
    ui -> your_table -> setColumnCount(6);
    ui->your_table ->setColumnWidth(0,static_cast<int>(width/14)); //Fecha
    ui->your_table ->setColumnWidth(1,static_cast<int>(width/14));  //Cliente
    ui->your_table ->setColumnWidth(2,static_cast<int>(width/14));  //Regional
    ui->your_table ->setColumnWidth(3,static_cast<int>(width/8));  //Task
    ui->your_table ->setColumnWidth(4,static_cast<int>(width/13));  //Comentarios
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
    ui->general_table ->setColumnWidth(0,static_cast<int>(width/14)); //Fecha
    ui->general_table ->setColumnWidth(1,static_cast<int>(width/14));  //Cliente
    ui->general_table ->setColumnWidth(2,static_cast<int>(width/14));  //Regional
    ui->general_table ->setColumnWidth(3,static_cast<int>(width/8));  //Task
    ui->general_table ->setColumnWidth(4,static_cast<int>(width/13));  //Comentarios
    ui->general_table ->setColumnWidth(5,static_cast<int>(width/11));  //Asesor
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

void Notebook::receive_info(QString userName, QString realName, QString token, QString url)
{
    this -> userName = userName;
    this -> realName = realName;
    this -> token   = token;
    this -> url = url;

    read_info();

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

void Notebook::read_info(){
    QNetworkAccessManager* nam = new QNetworkAccessManager (this);

    connect (nam, &QNetworkAccessManager::finished, this, [&](QNetworkReply* reply) {

        QByteArray resBin = reply->readAll ();

        if (reply->error ()) {
            QJsonDocument errorJson = QJsonDocument::fromJson (resBin);
            information_box("x","Error",QString::fromStdString (errorJson.toJson ().toStdString ()));
            return;
        }

        QJsonDocument okJson = QJsonDocument::fromJson (resBin);
        foreach (QJsonValue registro, okJson.object ().value ("tasks").toArray ()) {

            QHash<QString, QString> current;
            current.insert ("id_register", registro.toObject ().value ("_id").toString ());
            current.insert ("date", QDateTime::fromMSecsSinceEpoch(registro.toObject ().value ("registerDate").toVariant().toLongLong()).toString());
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
                db_your_tasks.insert(registro.toObject ().value("_id").toString(), current);
            }
            else{
                db_gral_tasks.insert(registro.toObject ().value("_id").toString(), current);
            }
         }
        update_gral_table(db_gral_tasks);
        update_your_table(db_your_tasks);
        reply->deleteLater ();
    });

    QNetworkRequest request;

    //change URL
    request.setUrl (QUrl ("http://"+this->url+"/tasks?completed=0"));

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

    }
}
