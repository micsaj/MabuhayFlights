#include "adminlogin.h"
#include "ui_adminlogin.h"
#include "adminaccess.h"
#include <QMessageBox>

AdminLogin::AdminLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminLogin)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
    setStyleSheet("AdminLogin { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");
}

AdminLogin::~AdminLogin()
{
    delete ui;
}

void AdminLogin::on_pushButton_clicked()
{
    QString username = ui->username->text();
        QString password = ui->password->text();

        if (username == "MabuhayFlights" && password == "admin123") {
            QMessageBox::information(this, "Login", "Login Successfully");

            // Hide the login dialog
            hide();

            // Create and show the AdminAccess dialog
            adminacc = new AdminAccess();
            adminacc->exec();
        } else {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Login");
            msgBox.setText("Login Incorrect");

            QPalette palette = msgBox.palette();
            palette.setColor(QPalette::Text, Qt::white);
            msgBox.setPalette(palette);

            msgBox.exec();
        }
}
