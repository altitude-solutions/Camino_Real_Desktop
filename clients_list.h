#ifndef CLIENTS_LIST_H
#define CLIENTS_LIST_H

#include <QWidget>
#include "new_client.h"
#include "update_client.h"

namespace Ui {
class Clients_list;
}

class Clients_list : public QWidget
{
    Q_OBJECT

public:
    explicit Clients_list(QWidget *parent = nullptr);
    ~Clients_list();

signals:
    void send_info(QString, QString);
    void send_contact(QHash<QString,QString>, QString, QString);

private slots:
    //Main info receiver
    void receive_info(QString, QString, QString, QString);

    //Information setters
    void read_client_info(QString);

    //buttons pressed
    void on_new_butt_2_clicked();
    void on_modify_butt_2_clicked();

    //Update App
    void update_client();
    void update_table(QHash<QString, QHash<QString, QString>>);

    //Table actions
    void on_table_clients_2_cellClicked(int row, int column);

    //Painter
    void paint_table(int);

    void on_lineEdit_2_editingFinished();

    void on_delete_butt_2_clicked();

private:
    Ui::Clients_list *ui;

    //Pointers
    New_client *new_cl;
    Update_client *update;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Main tables
    QHash<QString,QHash<QString,QString>>tabla_general;

    QHash<QString,QString>categorias;
    QHash<QString,QString>sucursales;
    QHash<QString,QString>cliente;

};

#endif // CLIENTS_LIST_H
