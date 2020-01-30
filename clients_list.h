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

    //buttons pressed
    void on_new_butt_2_clicked();
    void on_modify_butt_2_clicked();

    //Update App
    void update_client();

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

};

#endif // CLIENTS_LIST_H
