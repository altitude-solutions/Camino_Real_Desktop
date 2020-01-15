#ifndef MODIFY_H
#define MODIFY_H

#include <QWidget>
#include "information_box.h"

namespace Ui {
class Modify;
}

class Modify : public QWidget
{
    Q_OBJECT

public:
    explicit Modify(QWidget *parent = nullptr);
    ~Modify();

signals:
    void send_update();
    void send_info_box(QString, QString,QString, double, double);

private slots:
    void on_pushButton_clicked();
    void receive_contact(QHash<QString,QString>,QString, QString);

    void on_tarifario_butt_clicked();

    //Information Box
    void information_box(QString, QString, QString);

private:
    Ui::Modify *ui;

    QString token;
    QHash<QString,QString> data;
    QString url;

    Information_box *box_info;
};

#endif // MODIFY_H
