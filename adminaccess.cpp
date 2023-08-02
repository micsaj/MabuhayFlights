#include "adminaccess.h"
#include "ui_adminaccess.h"
#include "summaryreports.h"
#include "flightsummary.h"
#include "mainwindow.h"



AdminAccess::AdminAccess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminAccess)
{
    ui->setupUi(this);
    setStyleSheet("AdminAccess { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

}

AdminAccess::~AdminAccess()
{
    delete ui;
}

void AdminAccess::on_flights_clicked()
{
   flightsummary Flightsummary;
   Flightsummary.setModal(true);
   Flightsummary.exec();
}






void AdminAccess::on_summaryreports_clicked()
{
   reports Summary;
   Summary.setModal(true);
   Summary.exec();

}


void AdminAccess::on_home_clicked()
{
    close();

       // Show the main window
       MainWindow* mainWindow = new MainWindow();
       mainWindow->show();
}

