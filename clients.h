#ifndef CLIENTS_H
#define CLIENTS_H

#include <QWidget>
#include "new.h"
#include "modify.h"
#include "new.h"
#include "information_box.h"

namespace Ui {
class Clients;
}

class Clients : public QWidget
{
    Q_OBJECT

public:
    explicit Clients(QWidget *parent = nullptr);
    ~Clients();

signals:
    void send_info_box(QString, QString,QString, double, double);
    void send_contact(QHash<QString,QString>, QString, QString);
    void send_info(QString, QString);

private slots:
    void on_modify_butt_clicked();
    void read_client_info(QString);
    void on_new_butt_clicked();
    void information_box(QString, QString, QString);
    void receive_info(QString, QString, QString, QString);
    void update_table(QHash<QString, QHash<QString,QString>>);

    void on_table_clients_cellClicked(int row, int column);
    void paint_table( int);

    void update_client();
    void on_delete_butt_clicked();
    void on_lineEdit_editingFinished();

private:
    Ui::Clients *ui;

    Information_box *box_info;

    New *new_client;
    Modify *modify;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    QHash<QString, QHash<QString,QString>>db_clients;
    QHash<QString,QString> contact;

    //Actual Selection
    QHash<QString, QString>contact_data;

    QHash<QString,QHash<QString,QString>>tabla_sucursales;
    QHash<QString,QHash<QString,QString>>tabla_contactos;
    QHash<QString,QHash<QString,QString>>tabla_regionales;

    QHash<QString,QString>tabla_clientes;
    QHash<QString,QString>tabla_categorias;
    QHash<QString,QString>tabla_agentes;

    QHash<QString,QStringList>tabla_telefonos;
    QHash<QString,QStringList>tabla_mails;

};

#endif // CLIENTS_H
