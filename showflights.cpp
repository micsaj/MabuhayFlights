#include "mainwindow.h"
#include "showflights.h"
#include "ui_showflights.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>


ShowFlights::ShowFlights(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowFlights)
{
    ui->setupUi(this);
    setStyleSheet("ShowFlights { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

    connect(ui->domestic, SIGNAL(clicked()), this, SLOT(on_domestic_clicked()));
    connect(ui->international, SIGNAL(clicked()), this, SLOT(on_international_clicked()));

    // Create the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/micah/Documents/QT/PROJECTS/MFlights/mabuhayflights.db");

    // Check if the database was successfully opened
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database: " + db.lastError().text());
        return;
    }

}

ShowFlights::~ShowFlights()
{
    delete ui;
}

void ShowFlights::on_home_clicked()
{
    close();

    // Show the main window
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
}

void ShowFlights::on_domestic_clicked()
{
    // Create a QSqlQueryModel to populate the table view
    QSqlQueryModel* model = new QSqlQueryModel(this);
    QString query = "SELECT * FROM flights_dom_int WHERE flight_type = 'Domestic'";
    model->setQuery(query);

    // Set the model to the table view
    ui->tableView->setModel(model);
}

void ShowFlights::on_international_clicked()
{

    qDebug() << "International button clicked";

    // Create a QSqlQueryModel to populate the table view
        QSqlQueryModel* model = new QSqlQueryModel(this);
        QString query = "SELECT * FROM flights_dom_int WHERE flight_type = 'International'";
        model->setQuery(query);

        if (model->lastError().isValid()) {
            QMessageBox::critical(this, "Query Error", "Failed to execute query: " + model->lastError().text());
            return;
        }

        qDebug() << "Number of rows returned: " << model->rowCount();

        // Set the model to the table view
        ui->tableView->setModel(model);
}
