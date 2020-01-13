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

private:
    Ui::Nights *ui;
};

#endif // NIGHTS_H
