#ifndef INFO_H
#define INFO_H

#include <QWidget>

namespace Ui {
class Info;
}

class Info : public QWidget
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *);
    ~Info();

public slots:
    //Main info receiever
    void receive_text(QString);

private:
    Ui::Info *ui;
};

#endif // INFO_H
