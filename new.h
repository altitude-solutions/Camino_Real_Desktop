#ifndef NEW_H
#define NEW_H

#include <QWidget>

namespace Ui {
class New;
}

class New : public QWidget
{
    Q_OBJECT

public:
    explicit New(QWidget *parent = nullptr);
    ~New();

signals:
    void send_update();

private slots:
    void on_pushButton_clicked();
    void receive_info(QString,QString);
    void on_tarifario_butt_clicked();
    void send_id_contact(QString);
    void send_regional_id(QString);

private:
    Ui::New *ui;

    QString token;
    QString url;
};

#endif // NEW_H
