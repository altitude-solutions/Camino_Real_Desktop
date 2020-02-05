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

private slots:

    //Main info receiver
    void receiver(QString, QString, QString, QString);

private:
    Ui::Reservas *ui;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;
};

#endif // RESERVAS_H
