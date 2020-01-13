#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "page_1.h"
#include "page_2.h"
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    page_1 *p1;
    page_2 *p2;
    QVBoxLayout *l_a;
    QVBoxLayout *l_b;

    //Variable for the actual widget
    QString actual_widget;
};

#endif // MAINWINDOW_H
