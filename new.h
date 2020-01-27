#ifndef NEW_H
#define NEW_H

#include <QWidget>

namespace Ui {
class New;
}

class New : public QWidget
{
    Q_OBJECT

public:
    explicit New(QWidget *parent = nullptr);
    ~New();

signals:
    void send_update();

private slots:
    void on_pushButton_clicked();
    void receive_info(QString,QString);
    void on_tarifario_butt_clicked();

    void set_completer();
    void set_clients();
    void set_agents();

    void on_cliente_editingFinished();

    //Creating variables
    void create_client(QString);
    void create_city(QString);

    void on_sucursal_editingFinished();

private:
    Ui::New *ui;

    QString token;
    QString url;

    QHash<QString, QString>tabla_ciudades;
    QHash<QString, QString>tabla_agentes;
    QHash<QString, QString>tabla_clientes;

    //General variables
    QString id_cliente;
    QString id_ciudad;
    QString id_agentes;

    //Flags for variable creation
    QString client_flag;
    QString city_flag;

};

#endif // NEW_H
