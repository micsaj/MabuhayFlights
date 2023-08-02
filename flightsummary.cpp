#include "flightsummary.h"
#include "ui_flightsummary.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

flightsummary::flightsummary(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flightsummary)
{
    ui->setupUi(this);
    setStyleSheet("flightsummary { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

    // Check if the default connection already exists, reuse it
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    if (!db.isValid()) {
        // Set up the database connection if it doesn't exist
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/micah/Documents/QT/PROJECTS/MFlights/mabuhayflights.db");
    }

    // Check if the database was successfully opened
    if (!db.open()) {
        QMessageBox messageBox;
        messageBox.setStyleSheet("background-color: white; color: white;");
        messageBox.critical(this, "Database Error", "Failed to open database: " + db.lastError().text());
        return;
    }


}


flightsummary::~flightsummary()
{
    delete ui;
}


void flightsummary::on_showtable_clicked()
{
    // Create a QSqlTableModel
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("flights_dom_int");

    // Populate the model with data from the table
    model->select();

    // Set the model for the QTableView
    ui->tableView->setModel(model);

    // Optionally, you can resize the columns to fit the contents
    ui->tableView->resizeColumnsToContents();
}



void flightsummary::on_reset_clicked()
{
    // Get the selected row index from the QTableView
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();

    // Check if any row is selected
    if (selectedIndexes.isEmpty()) {
        // Display a message to inform the user to select a row first
        QMessageBox::information(this, "Selection Required", "Please select a row to reset available seats.");
        return;
    }

    // Assuming your QTableView's model is QSqlTableModel:
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->tableView->model());

    if (!model) {
        // If the model is not of QSqlTableModel type, show an error message and return
        QMessageBox::critical(this, "Data Error", "Failed to retrieve data. Please try again.");
        return;
    }

    // Get the row number of the selected index
    int row = selectedIndexes.at(0).row();

    // Retrieve the flight_id from the selected row
    QString selectedFlightID = model->data(model->index(row, 0)).toString();

    // Create a QSqlQuery to execute the update query
    QSqlQuery updateQuery;

    // Prepare the update query to reset available_seats to 30 for the selected flight_id
    QString updateQueryStr = "UPDATE flights_dom_int SET available_seats = 30 WHERE flight_id = :flightId";
    updateQuery.prepare(updateQueryStr);
    updateQuery.bindValue(":flightId", selectedFlightID);

    // Execute the update query
    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to reset available seats: " + updateQuery.lastError().text());
        return;
    }

    // Notify the user that the available seats have been reset
    QMessageBox::information(this, "Seats Reset", "Available seats have been reset to 30 for the selected flight.");
}



void flightsummary::on_add_clicked()
{
    // Reset all the UI elements to clear their content
        ui->arrival->clear();
        ui->departure->clear();
        ui->destination->clear();
        ui->flighttype->clear();
        ui->duration->clear();
        ui->flightid->clear();
        ui->flighttime->clear();
        ui->seats->clear();
        ui->ticketcost->clear();
}


void flightsummary::on_edit_clicked()
{

        // Get the selected row index from the QTableView
        QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();

        // Check if any row is selected
        if (selectedIndexes.isEmpty()) {
            // Display a message to inform the user to select a row first
            QMessageBox::information(this, "Selection Required", "Please select a row to modify.");
            return;
        }

        // Assuming your QTableView's model is QSqlTableModel:
        QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->tableView->model());

        if (model) {
            // Get the row number of the selected index
            int row = selectedIndexes.at(0).row();

            // Retrieve the data you want to modify from the model using the row index
            QString flightId = model->data(model->index(row, 0)).toString();
            QString arrivalTime = model->data(model->index(row, 1)).toString();
            QString departureTime = model->data(model->index(row, 2)).toString();
            int availableSeats = model->data(model->index(row, 3)).toInt();
            QString totalCost = model->data(model->index(row, 4)).toString();
            QString flightDuration = model->data(model->index(row, 5)).toString();
            QString flightType = model->data(model->index(row, 6)).toString();
            QString flightTime = model->data(model->index(row, 7)).toString();
            QString destinationId = model->data(model->index(row, 8)).toString();

            // Update the corresponding UI elements with the retrieved data
            ui->flightid->setText(flightId);
            ui->arrival->setText(arrivalTime);
            ui->departure->setText(departureTime);
            ui->seats->setText(QString::number(availableSeats));
            ui->ticketcost->setText(totalCost);
            ui->duration->setText(flightDuration);
            ui->flighttype->setText(flightType);
            ui->flighttime->setText(flightTime);
            ui->destination->setText(destinationId);

        }
}

void flightsummary::on_save_newflight_clicked()
{
    // Get the selected index from the QTableView
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty()) {
        // No row is selected, show an error message or return
        QMessageBox::warning(this, "No Row Selected", "Please select a row from the table to save the new flight data.");
        return;
    }

    // Get the row index of the selected item (the first column of the selected row)
    int rowIndex = selectedIndexes.at(0).row();

    // Get the flight_id from the model data using the row index and the corresponding column (flight_id is in column 0)
    QVariant selectedFlightIdData = ui->tableView->model()->data(ui->tableView->model()->index(rowIndex, 0));

    if (!selectedFlightIdData.isValid()) {
        // The flight_id data is invalid, show an error message or return
        QMessageBox::warning(this, "Invalid Data", "Selected row does not contain a valid flight_id.");
        return;
    }

    QString selectedFlightId = selectedFlightIdData.toString();

    // Retrieve the data from the QLineEdits
    QString flightId = ui->flightid->text();
    QString arrivalTime = ui->arrival->text();
    QString departTime = ui->departure->text();
    int availSeats = ui->seats->text().toInt(); // Convert to int
    QString ticketPrice = ui->ticketcost->text();
    QString flightDuration = ui->duration->text();
    QString destination = ui->destination->text();
    QString flightTime = ui->flighttime->text();
    QString flightType = ui->flighttype->text();

    // Add validation for required fields
    if (flightId.isEmpty() || arrivalTime.isEmpty() || departTime.isEmpty() || destination.isEmpty() || flightType.isEmpty()) {
        QMessageBox::warning(this, "Incomplete Data", "Please fill in all the required fields.");
        return;
    }

    // Check if the flight_id already exists in the database
    QSqlQuery checkExistingQuery;
    QString checkExistingQueryStr = "SELECT COUNT(*) FROM flights_dom_int WHERE flight_id = :flightId";
    checkExistingQuery.prepare(checkExistingQueryStr);
    checkExistingQuery.bindValue(":flightId", flightId);

    if (!checkExistingQuery.exec() || !checkExistingQuery.next()) {
        QMessageBox::critical(this, "Database Error", "Failed to check if the flight_id exists in the database: " + checkExistingQuery.lastError().text());
        return;
    }

    int rowCount = checkExistingQuery.value(0).toInt();

    if (rowCount > 0) {
        // The flight_id already exists in the database, show an error message or return
        QMessageBox::warning(this, "Flight Already Exists", "A flight with the same flight_id already exists in the database.");
        return;
    }

    // The flight_id is new, insert a new row in the database
    QSqlQuery insertQuery;
    QString insertQueryStr = "INSERT INTO flights_dom_int (flight_id, arrival_time, departure_time, available_seats, total_cost, flight_duration, flight_type, flight_time, "
                             "destination_id) "
                             "VALUES (:flightId, :arrivalTime, :departTime, :availSeats, :ticketPrice, "
                             ":flightDuration, :flightType, :flightTime, :destination)";

    insertQuery.prepare(insertQueryStr);
    insertQuery.bindValue(":flightId", flightId);
    insertQuery.bindValue(":arrivalTime", arrivalTime);
    insertQuery.bindValue(":departTime", departTime);
    insertQuery.bindValue(":availSeats", availSeats);
    insertQuery.bindValue(":ticketPrice", ticketPrice);
    insertQuery.bindValue(":flightDuration", flightDuration);
    insertQuery.bindValue(":flightType", flightType);
    insertQuery.bindValue(":flightTime", flightTime);
    insertQuery.bindValue(":destination", destination);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to insert data into the database: " + insertQuery.lastError().text());
        return;
    }

    // Notify the user that the new flight data has been saved
    QMessageBox::information(this, "Save Successful", "New flight data has been saved to the database.");
}




void flightsummary::on_del_clicked()
{
    // Get the selected row index from the QTableView
        QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();

        // Check if any row is selected
        if (selectedIndexes.isEmpty()) {
            // Display a message to inform the user to select a row first
            QMessageBox::information(this, "Selection Required", "Please select a row to delete.");
            return;
        }

        // Assuming your QTableView's model is QSqlTableModel:
        QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->tableView->model());

        if (model) {
            // Get the row number of the selected index
            int row = selectedIndexes.at(0).row();

            // Remove the selected row from the model
            model->removeRow(row);

            // Commit the changes to the database
            model->submitAll();
        }
}



void flightsummary::on_save_changes_clicked()
{
    // Get the selected row index from the QTableView
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();

    // Check if any row is selected
    if (selectedIndexes.isEmpty()) {
        // Display a message to inform the user to select a row first
        QMessageBox::information(this, "Selection Required", "Please select a row to save.");
        return;
    }

    // Assuming your QTableView's model is QSqlTableModel:
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->tableView->model());

    if (model) {
        // Get the row number of the selected index
        int row = selectedIndexes.at(0).row();

        // Retrieve the data you want to modify from the model using the row index
        QString flightId = model->data(model->index(row, 0)).toString();
        QString arrivalTime = ui->arrival->text();
        QString departureTime = ui->departure->text();
        int availableSeats = ui->seats->text().toInt();
        QString ticketPrice = ui->ticketcost->text();
        QString flightDuration = ui->duration->text();
        QString flightType = ui->flighttype->text();
        QString flightTime = ui->flighttime->text();
        QString destination = ui->destination->text();

        // Update the database with the modified data
        QSqlQuery updateQuery;
        QString updateQueryStr = "UPDATE flights_dom_int SET arrival_time = :arrivalTime, departure_time = :departureTime, "
                                 "available_seats = :availableSeats, total_cost = :ticketPrice, flight_duration = :flightDuration, flight_type = :flightType, "
                                 "flight_time = :flightTime, destination_id = :destination "
                                 "WHERE flight_id = :flightId";
        updateQuery.prepare(updateQueryStr);
        updateQuery.bindValue(":flightId", flightId);
        updateQuery.bindValue(":arrivalTime", arrivalTime);
        updateQuery.bindValue(":departureTime", departureTime);
        updateQuery.bindValue(":availableSeats", availableSeats);
        updateQuery.bindValue(":ticketPrice", ticketPrice);
        updateQuery.bindValue(":flightDuration", flightDuration);
        updateQuery.bindValue(":flightType", flightType);
        updateQuery.bindValue(":flightTime", flightTime);
        updateQuery.bindValue(":destination", destination);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to update data in the database: " + updateQuery.lastError().text());
            return;
        }

        // Notify the user that the changes have been saved
        QMessageBox::information(this, "Save Successful", "Changes have been saved to the database.");
    }
}



