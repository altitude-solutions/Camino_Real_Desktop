#ifndef PAGE_1_H
#define PAGE_1_H

#include <QWidget>
#include "nights.h"

namespace Ui {
class page_1;
}

class page_1 : public QWidget
{
    Q_OBJECT

public:
    explicit page_1(QWidget *parent = nullptr);
    ~page_1();

private slots:
    void on_pushButton_7_clicked();

private:
    Ui::page_1 *ui;

    Nights *nights;
};

#endif // PAGE_1_H
