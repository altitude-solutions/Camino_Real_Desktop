#ifndef PAGE_2_H
#define PAGE_2_H

#include <QWidget>
#include "extrainfo.h"
#include "nights.h"
#include "information_box.h"

namespace Ui {
class page_2;
}

class page_2 : public QWidget
{
    Q_OBJECT

public:
    explicit page_2(QWidget *parent = nullptr);
    ~page_2();

signals:
    void send_page(QString);
    void send_info_box(QString, QString,QString, double, double);
    void send_clients();

private slots:
    void on_pushButton_15_clicked();
    void on_email_butt_clicked();
    void on_visita_butt_clicked();
    void on_telefono_butt_clicked();
    void on_wpp_butt_clicked();
    void on_seguimiento_butt_clicked();
    void on_fidelizacion_butt_clicked();
    void on_prospeccion_butt_clicked();
    void on_bajomov_butt_clicked();
    void on_primer_butt_clicked();
    void on_tarifario_butt_clicked();
    void on_cotizacion_butt_clicked();
    void on_nuevacot_butt_clicked();
    void on_reserva_butt_clicked();
    void on_no_interesa_clicked();

    void paint_via(QString);
    void paint_motivo(QString);
    void paint_resultado(QString);

    void receiver(QString, QString, QString, QString);

    void receive_nights(QString);
    void receive_extra_info(QString, QString, QString);

    void read_client_info();
    void on_cliente_editingFinished();
    void on_icon_search_clicked();
    void on_pushButton_14_clicked();

    //Restart
    void restart();

    //Information Box
    void information_box(QString, QString, QString);

    void on_icon_pluss_clicked();

private:
    Ui::page_2 *ui;
    ExtraInfo *extraInfo;
    Nights *nights;
    Information_box *box_info;

    QString via;
    QString motivo;
    QString resultado;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //In case of reservation
    QString n_nights;

    //Extra inforamtion
    QString competencia;
    QString tarifa;
    QString noches;

    //Information of the clients
    QHash<QString, QHash<QString, QString>> db_clients;
};

#endif // PAGE_2_H
