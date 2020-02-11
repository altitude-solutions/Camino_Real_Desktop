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
    //main info receiver
    void receive_info(QString, QString);

    //Buttons pressed
    void on_cancel_butt_clicked();
    void on_guardar_butt_clicked();

    //Completer setters
    void set_city();
    void set_clients();

    //creators
    void create_contact();
    void append_contacto();

    //Combo box modification
    void on_regional_box_currentIndexChanged(const QString &arg1);
    void on_cliente_currentIndexChanged(const QString &arg1);

    void on_birthday_editingFinished();

private:
    Ui::New *ui;

    //main variables
    QString token;
    QString url;

    //cities completer
    QStringList completer_list;

    //Main Tables
    QHash<QString,QHash<QString, QString>>vinculo_categorias;
    QHash<QString,QHash<QString, QString>>tabla_vinculo;
    QHash<QString, QString>tabla_ciudades;
    QHash<QString, QString>tabla_clientes;

    //Id Variables
    QString id_regional;
    QString id_contacto;

};

#endif // NEW_H
