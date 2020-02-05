#ifndef RECORDS_H
#define RECORDS_H

#include <QWidget>
#include <QVBoxLayout>
#include "no_task.h"
#include "reservas.h"
#include "tasks.h"
#include "information_box.h"

namespace Ui {
class Records;
}

class Records : public QWidget
{
    Q_OBJECT

public:
    explicit Records(QWidget *parent = nullptr);
    ~Records();

signals:
    //To children
    void send_info(QString, QString, QString, QString);

    //To the information box
    void send_info_box(QString, QString,QString, double, double);

private slots:
    //Main info receiver
    void receive_info(QString, QString, QString, QString);

    //Pagues
    void on_page_1_clicked();
    void on_page_2_clicked();
    void on_page_3_clicked();

    //Information Box
    void information_box(QString, QString, QString);

private:
    Ui::Records *ui;

    //Pointers
    QVBoxLayout *l_a;

    //pages
    No_task *notask;
    Tasks *tasks;
    Reservas *reservas;

    //Info Variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Pages
    QString actual_widget;

    //Information from children
    Information_box *box_info;
};

#endif // RECORDS_H
