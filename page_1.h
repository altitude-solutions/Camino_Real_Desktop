#ifndef PAGE_1_H
#define PAGE_1_H

#include <QWidget>
#include "nights.h"
#include "information_box.h"

namespace Ui {
class page_1;
}

class page_1 : public QWidget
{
    Q_OBJECT

public:
    explicit page_1(QWidget *parent = nullptr);
    ~page_1();

signals:
    void send_page(QString);
    void send_clients();

    void send_nights_back();
    void send_info_box(QString, QString, QString);

private slots:
    void on_mail_butt_clicked();
    void on_visita_butt_clicked();
    void on_telefono_butt_clicked();
    void on_wpp_butt_clicked();
    void on_tarifario_butt_clicked();
    void on_cotizacion_butt_clicked();
    void on_reserva_butt_clicked();
    void on_otros_butt_clicked();

    void paint_via(QString);
    void paint_motivo(QString);

    void receiver(QString, QString, QString, QString);
    void receive_nights(QString);

    //Read info from the database
    void read_client_info();
    void on_cliente_editingFinished();
    void on_icon_search_clicked();
    void on_pushButton_9_clicked();

    //Restart
    void restart();

    //Information Box
    void information_box(QString, QString, QString);
    void on_icon_pluss_clicked();
    void on_contacto_drop_currentTextChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_reclamo_clicked();

private:
    Ui::page_1 *ui;

    Nights *nights;
    Information_box *box_info;

    QString via;
    QString motivo;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //In case of recervation
    QString n_nights;

    //Information of the clients
    QHash<QString, QHash<QString, QString>> db_clients;

    //From the database
    QHash<QString, QHash<QString, QString>>clientes;
    QHash<QString, QString>regionales;
    QHash<QString, QHash<QString, QString>>contactos;

    //To send to the database
    QString contact_name;
    QString contact_phone;
    QString contact_mail;
    QString contact_job;
    QString cid;

    //Actual Selection
    QHash<QString, QString>contact_data;
};

#endif // PAGE_1_H
