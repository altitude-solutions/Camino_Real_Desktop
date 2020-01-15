#ifndef INOUT_H
#define INOUT_H

#include <QWidget>
#include "page_1.h"
#include "page_2.h"
#include <QBoxLayout>

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

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void receive_info(QString, QString, QString, QString);
    void receive_clients();

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

};

#endif // INOUT_H
