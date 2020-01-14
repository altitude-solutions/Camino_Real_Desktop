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

signals:
    void send_info_box(QString, QString,QString, double, double);

public:
    Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void retreive_data(QString, QString, QString, QString);

private slots:
    void on_pushButton_clicked();
    void closeSession();
    void read_url();

    //Information Box
    void information_box(QString, QString, QString);

private:
    Ui::Login *ui;
    Information_box *box_info;

    MainWindow main_window;
    QString url;
};
#endif // LOGIN_H
