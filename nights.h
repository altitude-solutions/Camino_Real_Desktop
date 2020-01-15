#ifndef NIGHTS_H
#define NIGHTS_H

#include <QWidget>
#include "information_box.h"

namespace Ui {
class Nights;
}

class Nights : public QWidget
{
    Q_OBJECT

signals:
    void send_nights(QString);
    void send_info_box(QString, QString,QString, double, double);

public:
    explicit Nights(QWidget *parent = nullptr);
    ~Nights();

private slots:
    void on_pushButton_9_clicked();
    void receive_page(QString);

    //Information Box
    void information_box(QString, QString, QString);

    void on_pushButton_clicked();

private:
    Ui::Nights *ui;
     Information_box *box_info;

    QString actual_page;
};

#endif // NIGHTS_H
