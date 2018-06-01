#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QMessageBox>
#include "creator.h"
#include <QFontDatabase>
#include <QDesktopServices>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);    
    int id = QFontDatabase::addApplicationFont(":/font/Roboto-Bold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont roboto(family);
    roboto.setPointSize(16);
    ui->label->setFont(roboto);
    //this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);


    this->setFixedSize(this->width(),this->height());


}

login::~login()
{
    delete ui;
}
void login::closeEvent(QCloseEvent* event)
{
    qApp->quit();
    // Q_UNUSED(event);
    // settings.setValue("window/geometry", saveGeometry());
}
void login::on_pushButton_clicked()
{
    qDebug() << "Login to App before proceed";
    qDebug() << ui->lineEdit->text() << ui->lineEdit_2->text();
    if(!QString(ui->lineEdit->text()).isEmpty() && !QString(ui->lineEdit_2->text()).isEmpty()){
        emit auth(ui->lineEdit->text(), ui->lineEdit_2->text());
        this->accept();
        this->hide();
    }else{
        QMessageBox msgBox(this);
        msgBox.setText(tr("Please provide login/password!"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, tr("OK"));
        msgBox.exec();
    }


}

void login::on_label_2_linkActivated(const QString &link)
{

}

void login::on_pushButton_2_clicked()
{

    QDesktopServices::openUrl(QUrl("http://www.example.com/"));
}
