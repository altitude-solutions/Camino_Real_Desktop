#ifndef NEW_CLIENT_H
#define NEW_CLIENT_H

#include <QWidget>

namespace Ui {
class New_client;
}

class New_client : public QWidget
{
    Q_OBJECT

signals:
    void send_update();

public:
    explicit New_client(QWidget *parent = nullptr);
    ~New_client();

private slots:
    //main info receiver
    void receive_info(QString, QString);

    //Buttons pressed
    void on_cancel_butt_clicked();
    void on_guardar_butt_clicked();

    //Table setters
    void set_city();
    void set_client();
    void set_agents();
    void set_category();

    //Create
    void create_client(QString);
    void create_city(QString);
    void create_regional();

    // Create synchronous
    void syncCreateClient (QString clientName, QString regionalID);
    QString syncCreateRegional ();

    //Append
    void append_region(QString);

    //Finish editing
    void on_cliente_editingFinished();
    void on_regional_2_editingFinished();
    void on_aniversario_editingFinished();

private:
    Ui::New_client *ui;

    //main variables
    QString token;
    QString url;

    //Main tables
    QHash<QString, QHash<QString, QString>>clientes_ciudad;
    QHash<QString, QHash<QString, QString>>clientes_categoria;

    QHash<QString, QString>tabla_agentes;
    QHash<QString, QString>tabla_clientes;
    QHash<QString, QString>tabla_ciudades;
    QHash<QString, QString>tabla_categorias;

};

#endif // NEW_CLIENT_H
