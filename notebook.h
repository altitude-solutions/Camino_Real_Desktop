#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QWidget>

namespace Ui {
class Notebook;
}

class Notebook : public QWidget
{
    Q_OBJECT

public:
    explicit Notebook(QWidget *parent = nullptr);
    ~Notebook();

private:
    Ui::Notebook *ui;
};

#endif // NOTEBOOK_H
