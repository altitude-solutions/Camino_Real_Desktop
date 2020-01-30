#ifndef CLIENTS_LIST_H
#define CLIENTS_LIST_H

#include <QWidget>

namespace Ui {
class Clients_list;
}

class Clients_list : public QWidget
{
    Q_OBJECT

public:
    explicit Clients_list(QWidget *parent = nullptr);
    ~Clients_list();

private slots:
    void receive_info(QString, QString, QString, QString);

private:
    Ui::Clients_list *ui;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

};

#endif // CLIENTS_LIST_H
