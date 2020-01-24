#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QWidget>
#include "information_box.h"

namespace Ui {
class Notebook;
}

class Notebook : public QWidget
{
    Q_OBJECT

public:
    explicit Notebook(QWidget *parent = nullptr);
    ~Notebook();

signals:
    void send_info_box(QString, QString,QString, double, double);

private slots:
    void receive_info(QString, QString, QString, QString);

    //Information Box
    void information_box(QString, QString, QString);
    void update_your_table(QHash<QString,QHash<QString, QString>>);
    void update_gral_table(QHash<QString,QHash<QString, QString>>);

    void read_gral_info(QString, QString, QString);
    void read_your_info(QString, QString);

    void all_your_info();
    void all_gral_info();

    void restart();

    void on_cliente_editingFinished();
    void on_regional_editingFinished();
    void on_cliente_2_editingFinished();
    void on_regional_2_editingFinished();
    void on_asesor_2_editingFinished();
    void on_delete_butt_clicked();

    void on_your_table_cellClicked(int row, int column);

    void paint_your_table(int);
    void paint_gral_table(int);

    void on_general_table_cellClicked(int row, int column);

    void on_delete_butt_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Notebook *ui;

    Information_box *box_info;

    //info variables
    QString userName;
    QString realName;
    QString token;
    QString url;

    //Filters
    QString your_client_filter;
    QString your_regional_filter;

    QString gral_client_filter;
    QString gral_regional_filter;
    QString gral_assistant_filter;

    QHash<QString,QHash<QString,QString>>your_filtered_tasks;
    QHash<QString,QHash<QString,QString>>gral_filtered_tasks;

    QHash<QString,QHash<QString,QString>>all_gral_tasks;
    QHash<QString,QHash<QString,QString>>all_your_tasks;

    QHash<QString, QString>your_reg;
    QHash<QString, QString>gral_reg;

};

#endif // NOTEBOOK_H
