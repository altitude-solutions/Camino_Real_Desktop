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
    void set_category();

    //Creating variables
    void create_client(QString);
    void create_city(QString);
    void create_sucursal();
    void create_contacto();

    //appends
    void append_to_sucursal();
    void append_to_client();

    //Finish Editing
    void on_sucursal_editingFinished();
    void on_cliente_editingFinished();
    void on_categoria_editingFinished();
    void on_agent_editingFinished();

    void restart();

private:
    Ui::New *ui;

    QString token;
    QString url;

    QHash<QString,QHash<QString, QString>>tabla_vinculo;
    QHash<QString,QHash<QString, QString>>vinculo_categorias;

    QHash<QString, QString>tabla_ciudades;
    QHash<QString, QString>tabla_agentes;
    QHash<QString, QString>tabla_clientes;
    QHash<QString, QString>tabla_categorias;

    //General variables
    QString id_cliente;
    QString id_ciudad;
    QString id_agentes;
    QString id_categoria;
    QString id_contacto;
    QString id_regional;

    //Flags for variable creation
    QString client_flag;
    QString city_flag;

    //Append flags
    bool append_sucursal;
    bool append_cliente;
};

#endif // NEW_H
