#ifndef INOUT_H
#define INOUT_H

#include <QWidget>
#include "page_1.h"
#include "page_2.h"
#include <QBoxLayout>
#include "nights.h"
#include "information_box.h"
#include "extrainfo.h"

namespace Ui {
class InOut;
}

class InOut : public QWidget
{
    Q_OBJECT

public:
    explicit InOut(QWidget *parent = nullptr);
    ~InOut();

signals:
    void send_info(QString, QString, QString, QString);
    void send_clients();

    //to send the info
    void send_page(QString);
    void send_nights_back(QString, QString, QString, QString);

    //to send extra info
    void send_info_back(QString, QString, QString);

    //To the information box
    void send_info_box(QString, QString,QString, double, double);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void receive_info(QString, QString, QString, QString);
    void receive_clients();

    //Variables from children
    void activate_nights();
    void receive_nights(QString, QString, QString, QString);

    //variables for extra information
    void activate_info();
    void receive_info(QString, QString, QString);

    //information box
    void information_box(QString, QString, QString);

private:
    Ui::InOut *ui;

    page_1 *p1;
    page_2 *p2;

    QVBoxLayout *l_a;

    //Variable for the actual widget
    QString actual_widget;

    //Info Variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Info from children
    Nights *nights;
    Information_box *box_info;
    ExtraInfo *extraInfo;
};

#endif // INOUT_H
