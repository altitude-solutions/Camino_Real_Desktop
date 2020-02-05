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
    void send_nights(QString, QString, QString, QString);
    void send_info_box(QString, QString,QString, double, double);

public:
    explicit Nights(QWidget *parent = nullptr);
    ~Nights();

private slots:

    //Main information receiver
    void receive_page(QString);

    //Information Box
    void information_box(QString, QString, QString);

    //Buttons pressed
    void on_pushButton_clicked();
    void on_pushButton_9_clicked();

    //Validators at the end of editing
    void on_nights_2_editingFinished();
    void on_nights_3_editingFinished();
    void on_nights_4_editingFinished();
    void on_nights_editingFinished();

private:
    Ui::Nights *ui;
     Information_box *box_info;

    QString actual_page;
};

#endif // NIGHTS_H
