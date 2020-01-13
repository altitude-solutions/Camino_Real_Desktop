#ifndef EXTRAINFO_H
#define EXTRAINFO_H

#include <QWidget>

namespace Ui {
class ExtraInfo;
}

class ExtraInfo : public QWidget
{
    Q_OBJECT

signals:
    void  send_info(QString, QString, QString);

public:
    explicit ExtraInfo(QWidget *parent = nullptr);
    ~ExtraInfo();

private slots:
    void on_pushButton_9_clicked();

private:
    Ui::ExtraInfo *ui;
};

#endif // EXTRAINFO_H
