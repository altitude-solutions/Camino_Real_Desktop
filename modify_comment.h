#ifndef MODIFY_COMMENT_H
#define MODIFY_COMMENT_H

#include <QWidget>

namespace Ui {
class Modify_comment;
}

class Modify_comment : public QWidget
{
    Q_OBJECT

public:
    explicit Modify_comment(QWidget *parent = nullptr);
    ~Modify_comment();

signals:
    void update();
    void send_info(QString,QString,QString);

private slots:
    void receive_comment(QString, QString, QString, QString);
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::Modify_comment *ui;

    QString comments;
    QString id_register;
    QString token;
    QString url;
};

#endif // MODIFY_COMMENT_H
