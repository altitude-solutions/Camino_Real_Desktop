#ifndef LEAD_INFORMATION_H
#define LEAD_INFORMATION_H

#include <QWidget>

namespace Ui {
class Lead_information;
}

class Lead_information : public QWidget
{
    Q_OBJECT

public:
    explicit Lead_information(QWidget *parent = nullptr);
    ~Lead_information();

signals:
    void send_lead(QString, QString, QString);

private slots:
    void on_pushButton_9_clicked();

private:
    Ui::Lead_information *ui;
};

#endif // LEAD_INFORMATION_H
