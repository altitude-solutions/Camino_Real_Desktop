#ifndef UPDATE_CLIENT_H
#define UPDATE_CLIENT_H

#include <QWidget>

namespace Ui {
class Update_client;
}

class Update_client : public QWidget
{
    Q_OBJECT

public:
    explicit Update_client(QWidget *parent = nullptr);
    ~Update_client();

signals:
    void send_update();

private slots:
    //main info receiver
    void receive_contact(QHash<QString,QString>, QString, QString);

    void on_cancel_butt_clicked();

private:
    Ui::Update_client *ui;

    //Main info
    QHash<QString,QString> data;
    QString token;
    QString url;
};

#endif // UPDATE_CLIENT_H
