#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include "adminlogin.h"
#include "bookflights.h"
#include "showflights.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    setStyleSheet("MainWindow { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::showTime);
    timer->start(1000); // Timer interval in milliseconds (e.g., update every second)

    showTime(); // Display initial time

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeText = currentDateTime.toString();
    ui->timeanddate->setText(dateTimeText);

    // Create instances of the dialogs
    bookflightsDialog = new BookFlights(this);
    adminloginDialog = new AdminLogin(this);
    showflightsDialog = new ShowFlights(this);

    // Connect signals to slots for dialog handling
    connect(bookflightsDialog, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
    connect(adminloginDialog, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
    connect(showflightsDialog, SIGNAL(dialogOpened()), this, SLOT(onDialogOpened()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeText = currentDateTime.toString();

    ui->timeanddate->setText(dateTimeText);
}

void MainWindow::on_BookFlights_clicked()
{
    // Show the book flights dialog
    emit onDialogOpened();
    BookFlights bookflights;
        if (bookflights.exec() == QDialog::Accepted)
        {
            // Show the main window
            show();
        }
}

void MainWindow::on_admin_clicked()
{
    // Show the admin login dialog
    emit onDialogOpened();
    adminloginDialog->exec();
}

void MainWindow::on_ShowFlights_clicked()
{
    // Show the show flights dialog
    emit onDialogOpened();
    showflightsDialog->exec();
}

void MainWindow::onDialogOpened()
{
    // Hide the main window when a dialog is open
    hide();
}
