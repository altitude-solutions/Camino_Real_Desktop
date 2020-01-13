#ifndef WIDGET_MAIN_H
#define WIDGET_MAIN_H

#include <QWidget>

namespace Ui {
class widget_main;
}

class widget_main : public QWidget
{
    Q_OBJECT

public:
    explicit widget_main(QWidget *parent = nullptr);
    ~widget_main();

private:
    Ui::widget_main *ui;


};

#endif // WIDGET_MAIN_H
