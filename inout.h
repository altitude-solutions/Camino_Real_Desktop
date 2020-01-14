#ifndef INOUT_H
#define INOUT_H

#include <QWidget>

namespace Ui {
class InOut;
}

class InOut : public QWidget
{
    Q_OBJECT

public:
    explicit InOut(QWidget *parent = nullptr);
    ~InOut();

private:
    Ui::InOut *ui;
};

#endif // INOUT_H
