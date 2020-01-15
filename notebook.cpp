#include "notebook.h"
#include "ui_notebook.h"
#include <QScreen>

Notebook::Notebook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notebook)
{
    ui->setupUi(this);

    //Get screen Size
   const auto screens = qApp->screens();

   int width = screens[0]->geometry().width();
   int height = screens[0]->geometry().height();

    //set widget size dynamic, aspect ratio 16:9
    double size_w = (width);
    double size_h = (height*0.9);
    QSize size (static_cast<int>(size_w), static_cast<int>(size_h));

    this->setFixedSize(size);

}

Notebook::~Notebook()
{
    delete ui;
}
