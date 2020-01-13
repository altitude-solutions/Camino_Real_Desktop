#ifndef PAGE_2_H
#define PAGE_2_H

#include <QWidget>
#include "extrainfo.h"
#include "nights.h"

namespace Ui {
class page_2;
}

class page_2 : public QWidget
{
    Q_OBJECT

public:
    explicit page_2(QWidget *parent = nullptr);
    ~page_2();

private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::page_2 *ui;
    ExtraInfo *extraInfo;
    Nights *nights;
};

#endif // PAGE_2_H
