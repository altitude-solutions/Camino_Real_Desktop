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

private slots:
    void on_pushButton_clicked();

private:
    Ui::New *ui;
};

#endif // NEW_H
