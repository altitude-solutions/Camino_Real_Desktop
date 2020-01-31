#ifndef UPDATE_CLIENT_H
#define UPDATE_CLIENT_H

#include <QWidget>

namespace Ui {
class Update_client;
}

class Update_client : public QWidget
{
    Q_OBJECT

public:
    explicit Update_client(QWidget *parent = nullptr);
    ~Update_client();

signals:
    void send_update();

private slots:
    //main info receiver
    void receive_contact(QHash<QString,QString>, QString, QString);

    //Buttons pressed
    void on_cancel_butt_clicked();
    void on_guardar_butt_clicked();

    //ompleter setters
    void set_category();
    void set_agents();

    //Label finished
    void on_aniversario_editingFinished();

private:
    Ui::Update_client *ui;

    //Main info
    QHash<QString,QString> data;
    QString token;
    QString url;

    //Main Variables
    QString id_client;
    QString id_sucursal;
    QString id_ciudad;
    QString id_categoria;
    QString id_agente;

    //Main tables
    QHash<QString, QString>tabla_agentes;
    QHash<QString, QString>tabla_categorias;

};

#endif // UPDATE_CLIENT_H
