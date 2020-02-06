#ifndef RESERVAS_H
#define RESERVAS_H

#include <QWidget>

namespace Ui {
class Reservas;
}

class Reservas : public QWidget
{
    Q_OBJECT

public:
    explicit Reservas(QWidget *parent = nullptr);
    ~Reservas();

signals:
    void send_info_box(QString, QString, QString);
    void send_benefits_id(QString, QString, QString, QString, QString);

private slots:
    //Main info receiver
    void receiver(QString, QString, QString, QString);

    //Read information
    void read_info(QString);

    //Update table
    void update_table(QHash<QString, QHash<QString,QString>>);
    void updater();

    //Count pages
    void define_pages(int);

    void on_pages_activated(const QString &arg1);
    void on_table_clients_2_cellClicked(int row, int column);
    void on_modify_clicked();

private:
    Ui::Reservas *ui;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Main task table
    QHash<QString, QHash<QString,QString>>tabla_informacion;

    //Pages control
    bool first_time;

    //Register to modify
    QString id_register;
    QString early;
    QString late;
    QString upgrade;
    QString noshow;
};

#endif // RESERVAS_H
