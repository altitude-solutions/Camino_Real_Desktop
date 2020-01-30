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

private slots:    
    //main info receiver
    void receive_contact(QHash<QString,QString>, QString, QString);

    //Buttons pressed
    void on_cancel_butt_clicked();
    void on_guardar_butt_clicked();

    //Restart widget
    void restart();

private:
    Ui::Modify *ui;

    //Main info
    QString token;
    QString url;
    QString id_contacto;
};

#endif // MODIFY_H
