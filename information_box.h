#ifndef INFORMATION_BOX_H
#define INFORMATION_BOX_H

#include <QWidget>

namespace Ui {
class Information_box;
}

class Information_box : public QWidget
{
    Q_OBJECT

public:
    explicit Information_box(QWidget *parent = nullptr);
    ~Information_box();

private slots:
    void  receive_info(QString, QString, QString, double, double);

    void on_pushButton_14_clicked();

private:
    Ui::Information_box *ui;
};

#endif // INFORMATION_BOX_H
