#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
signals:
    void auth(QString u, QString p);

private slots:
    void on_pushButton_clicked();

public:
    Ui::login *ui;
};

#endif // LOGIN_H
