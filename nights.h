#ifndef NIGHTS_H
#define NIGHTS_H

#include <QWidget>

namespace Ui {
class Nights;
}

class Nights : public QWidget
{
    Q_OBJECT

signals:
    void send_nights(QString);

public:
    explicit Nights(QWidget *parent = nullptr);
    ~Nights();

private slots:
    void on_pushButton_9_clicked();
    void receive_page(QString);

private:
    Ui::Nights *ui;

    QString actual_page;
};

#endif // NIGHTS_H
