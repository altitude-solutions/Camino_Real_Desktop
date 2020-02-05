#ifndef NO_TASK_H
#define NO_TASK_H

#include <QWidget>

namespace Ui {
class No_task;
}

class No_task : public QWidget
{
    Q_OBJECT

signals:
    void send_info_box(QString, QString, QString);

public:
    explicit No_task(QWidget *parent = nullptr);
    ~No_task();



private slots:
    //Main info receiver
    void receiver(QString, QString, QString, QString);

    //Main info reader
    void read_info(QString, QString);

    //Update table
    void update_table(QHash<QString, QHash<QString,QString>>);

    //Define number of pages
    void define_pages(int);

    void on_pages_activated(const QString &arg1);
    void on_b_Box_activated(const QString &arg1);

private:
    Ui::No_task *ui;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Information table
    QHash<QString, QHash<QString, QString>>tabla_informacion;

    //Auxiliar, just to shot pages once
    bool first_time = true;
};

#endif // NO_TASK_H
