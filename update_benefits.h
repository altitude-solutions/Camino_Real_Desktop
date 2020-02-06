#ifndef UPDATE_BENEFITS_H
#define UPDATE_BENEFITS_H

#include <QWidget>

namespace Ui {
class Update_benefits;
}

class Update_benefits : public QWidget
{
    Q_OBJECT

public:
    explicit Update_benefits(QWidget *parent = nullptr);
    ~Update_benefits();

signals:
    void send_info_box(QString, QString, QString);
    void send_update();

private slots:
    void receive_info(QString, QString, QString, QString, QString, QString, QString, QString);

    void on_cancel_butt_clicked();

    void on_guardar_butt_clicked();

private:
    Ui::Update_benefits *ui;

    //info variables
    QString early;
    QString late;
    QString upgrade;
    QString noshow;
    QString userName;
    QString register_id;
    QString token;
    QString url;
};

#endif // UPDATE_BENEFITS_H
