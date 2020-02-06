#ifndef TASKS_H
#define TASKS_H

#include <QWidget>

namespace Ui {
class Tasks;
}

class Tasks : public QWidget
{
    Q_OBJECT

signals:
    void send_info_box(QString, QString, QString);

public:
    explicit Tasks(QWidget *parent = nullptr);
    ~Tasks();

private slots:
    //Main info receiver
    void read_info(QString);

    //Main info receiver
    void receiver(QString, QString, QString, QString);

    //Update table
    void update_table(QHash<QString, QHash<QString,QString>>);

    //Define number of pages
    void define_pages(int);

    //Combo Box activated
    void on_pages_activated(const QString &arg1);

    void on_table_clients_2_cellClicked(int row, int column);

    void on_modify_butt_2_clicked();

private:
    Ui::Tasks *ui;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Information table
    QHash<QString, QHash<QString, QString>>tabla_informacion;

    //Auxiliar, just to shot pages once
    bool first_time = true;

    //Id of register to modify
    QString register_id;
};

#endif // TASKS_H
