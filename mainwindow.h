#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "page_1.h"
#include "page_2.h"
#include <QVBoxLayout>
#include "ham_options.h"
#include "notebook.h"
#include "inout.h"
#include "clients.h"
#include "clients_list.h"
#include "records.h"
#include "info.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void logOut();
    void send_info(QString, QString, QString, QString);
    void send_text_info(QString);

private slots:
    void on_icon_back_clicked();
    void receive_info(QString, QString, QString, QString);
    void close_session();
    void show_notebook();
    void show_inout();
    void show_clients();
    void show_clients_list();
    void show_records();

    //Show extra info
    void receive_extra_info(QString);

private:
    Ui::MainWindow *ui;

    //pages
    Notebook *notebook;
    InOut *inOut;
    Clients *clients;
    Clients_list *clientsList;
    Ham_options *ham;
    QVBoxLayout *l_a;
    Records *records;
    Info *information;

    //Variable for the actual widget
    QString actual_widget;

    //Info Variables
    QString userName;
    QString realName;
    QString token;
    QString url;
};

#endif // MAINWINDOW_H
