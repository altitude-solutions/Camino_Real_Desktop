#ifndef RECORDS_H
#define RECORDS_H

#include <QWidget>

namespace Ui {
class Records;
}

class Records : public QWidget
{
    Q_OBJECT

public:
    explicit Records(QWidget *parent = nullptr);
    ~Records();

private slots:
    //Main info receiver
    void receive_info(QString, QString, QString, QString);

private:
    Ui::Records *ui;

    //Info Variables
    QString userName;
    QString realName;
    QString token;
    QString url;
};

#endif // RECORDS_H
