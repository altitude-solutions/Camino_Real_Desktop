#ifndef HAM_OPTIONS_H
#define HAM_OPTIONS_H

#include <QWidget>

namespace Ui {
class Ham_options;
}

class Ham_options : public QWidget
{
    Q_OBJECT

public:
    explicit Ham_options(QWidget *parent = nullptr);
    ~Ham_options();
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);

signals:
    void getOut();
    void send_notebook();
    void send_inout();
    void send_clients();
    void send_clients_list();
    void send_records();

private slots:
    void on_pushButton_9_clicked();
    void on_close_seesion_clicked();
    void on_pushButton_10_clicked();
    void on_clients_clicked();
    void on_contactos_clicked();
    void on_records_clicked();

private:
    Ui::Ham_options *ui;
};

#endif // HAM_OPTIONS_H
