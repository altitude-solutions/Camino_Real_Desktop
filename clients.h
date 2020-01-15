#ifndef CLIENTS_H
#define CLIENTS_H

#include <QWidget>

namespace Ui {
class Clients;
}

class Clients : public QWidget
{
    Q_OBJECT

public:
    explicit Clients(QWidget *parent = nullptr);
    ~Clients();

private:
    Ui::Clients *ui;
};

#endif // CLIENTS_H
