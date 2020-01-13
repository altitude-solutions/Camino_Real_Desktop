#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <mainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Login *ui;

    MainWindow main_window;
};
#endif // LOGIN_H
