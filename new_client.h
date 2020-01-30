#ifndef NEW_CLIENT_H
#define NEW_CLIENT_H

#include <QWidget>

namespace Ui {
class New_client;
}

class New_client : public QWidget
{
    Q_OBJECT

signals:
    void send_update();

public:
    explicit New_client(QWidget *parent = nullptr);
    ~New_client();

private slots:
    //main info receiver
    void receive_info(QString, QString);

    //Buttons pressed
    void on_cancel_butt_clicked();

private:
    Ui::New_client *ui;

    //main variables
    QString token;
    QString url;
};

#endif // NEW_CLIENT_H
