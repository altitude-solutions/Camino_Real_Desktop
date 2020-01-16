#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QWidget>
#include "information_box.h"

namespace Ui {
class Notebook;
}

class Notebook : public QWidget
{
    Q_OBJECT

public:
    explicit Notebook(QWidget *parent = nullptr);
    ~Notebook();

signals:
    void send_info_box(QString, QString,QString, double, double);

private slots:
    void receive_info(QString, QString, QString, QString);

    //Information Box
    void information_box(QString, QString, QString);
    void update_your_table(QHash<QString,QHash<QString, QString>>);
    void update_gral_table(QHash<QString,QHash<QString, QString>>);
    void read_info();

private:
    Ui::Notebook *ui;

    Information_box *box_info;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    QHash<QString,QHash<QString,QString>>db_your_tasks;
    QHash<QString,QHash<QString,QString>>db_gral_tasks;

};

#endif // NOTEBOOK_H
