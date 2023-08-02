#include "summaryreports.h"
#include "ui_summaryreports.h"
#include "salesreport.h"
#include "flightoccupancy.h"
#include "schedulereport.h"
#include "pmr.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

reports::reports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reports)
{
    ui->setupUi(this);
    setStyleSheet("reports { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

    // Establish the database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/micah/Documents/QT/PROJECTS/MFlights/mabuhayflights.db");

    // Check if the database was successfully opened
    if (!db.open()) {
        // Show an error message if the database failed to open
        QMessageBox messageBox;
        messageBox.setStyleSheet("background-color: white; color: black;");
        messageBox.setWindowTitle("Database Error");
        messageBox.setText("Failed to open the database: " + db.lastError().text());
        messageBox.exec();
    }
}
reports::~reports()
{
    delete ui;
}

void reports::on_flightoccupancy_clicked()
{
    QSqlQuery query;
    QString queryString = "SELECT flight_id, destination, total_seats, booked_seats, availableseats "
                          "FROM flight_occupancy";

    // Set the query to fetch all rows
    query.setForwardOnly(false);

    if (query.exec(queryString)) {
        // Create and open the Flightoccupancy dialog
        flightoccupancy Flightoccupancy;
        Flightoccupancy.setModal(true);

        // Process each flight's data
        QString reportText = "Flight Details\n";

        while (query.next()) {
            QString flightNumber = query.value("flight_id").toString();
            QString destination = query.value("destination").toString();
            int totalSeats = query.value("total_seats").toInt();
            int bookedSeats = query.value("booked_seats").toInt();
            int availableSeats = query.value("availableseats").toInt();
            double occupancyRate = (static_cast<double>(bookedSeats) / totalSeats) * 100;

            // Generate the report text for each flight
            reportText += "\nFlight Number: " + flightNumber + "\n";
            reportText += "Destination: " + destination + "\n\n";
            reportText += "Total Seats: " + QString::number(totalSeats) + "\n";
            reportText += "Booked Seats: " + QString::number(bookedSeats) + "\n";
            reportText += "Available Seats: " + QString::number(availableSeats) + "\n";
            reportText += "Occupancy Rate: " + QString::number(occupancyRate, 'f', 2) + "%\n";
        }

        // Set the final report content and open the dialog
        Flightoccupancy.setReportContent(reportText);
        Flightoccupancy.exec();
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve flight occupancy data from the database: " + query.lastError().text());
    }
}



void reports::on_schedulereport_clicked()
{
    schedulereport Schedreport;
    Schedreport.setModal(true);
    Schedreport.exec();
}

void reports::on_salesreport_clicked()
{
    QSqlQuery query;
    QString queryString = "SELECT flight_id, destination, tickets_sold, revenue "
                          "FROM ticket_sales";

    if (query.exec(queryString)) {
        // Create and open the SalesReport dialog
        salesreport Sales;
        Sales.setModal(true);

        // Process each flight's data and generate the report text
        QString reportText = "DOMESTIC FLIGHTS\n\n";

        while (query.next()) {
            QString flightNumber = query.value("flight_id").toString();
            QString destination = query.value("destination").toString();
            int ticketsSold = query.value("tickets_sold").toInt();

            double revenue = query.value("revenue").toDouble();
            QString flightType = flightNumber.at(0).toUpper(); // Get the initial letter of the flight number

            if (flightType == "D") {
                // Generate the report text for each domestic flight
                reportText += "Flight Number: " + flightNumber + "\n";
                reportText += "Destination: " + destination + "\n";
                reportText += "Tickets Sold: " + QString::number(ticketsSold) + "\n";
                reportText += "Revenue: ₱" + QString::number(revenue) + "\n";
                reportText += "\n";
            }
        }

        // Add a separator between domestic and international flights
        reportText += "----------------------------------------------------------------------------\n";
        reportText += "INTERNATIONAL FLIGHTS\n\n";

        // Reset the query to process international flights
        query.seek(-1);

        while (query.next()) {
            QString flightNumber = query.value("flight_id").toString();
            QString destination = query.value("destination").toString();
            int ticketsSold = query.value("tickets_sold").toInt();
            double ticketPrice = query.value("ticket_price").toDouble();
            double revenue = query.value("revenue").toDouble();
            QString flightType = flightNumber.at(0).toUpper(); // Get the initial letter of the flight number

            if (flightType == "I") {
                // Generate the report text for each international flight
                reportText += "Flight Number: " + flightNumber + "\n";
                reportText += "Destination: " + destination + "\n";
                reportText += "Tickets Sold: " + QString::number(ticketsSold) + "\n";
                reportText += "Revenue: ₱" + QString::number(revenue) + "\n";
                reportText += "\n";
            }
        }

        // Set the final report content and open the dialog
        Sales.setReportContent(reportText);
        Sales.exec();
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve sales data from the database: " + query.lastError().text());
    }
}



void reports::on_pass_reports_clicked()
{
    // Retrieve the passenger information from the database
    QSqlQuery query;
    QString queryString = "SELECT flight_id, name, address, contactno, payment_method, destination FROM passenger_info";
    if (!query.exec(queryString)) {
        QMessageBox messageBox;
        messageBox.setStyleSheet("background-color: white; color: black;");
        messageBox.setWindowTitle("Database Error");
        messageBox.setText("Failed to retrieve passenger information from the database: " + query.lastError().text());
        messageBox.exec();
        return;
    }

    // Create a map to store the passenger details for each flight
    QMap<QString, QList<QStringList>> flightPassengersMap;

    // Iterate over the query result and populate the passenger details map
    while (query.next()) {
        QString flightId = query.value("flight_id").toString();
        QString name = query.value("name").toString();
        QString address = query.value("address").toString();
        QString contactNo = query.value("contactno").toString();
        QString paymentMethod = query.value("payment_method").toString();
        QString destination = query.value("destination").toString(); // Retrieve destination

        QStringList passengerDetails;
        passengerDetails << name << address << contactNo << paymentMethod << destination; // Add destination to passenger details

        flightPassengersMap[flightId].append(passengerDetails);
    }

    // Create the passenger manifest report content
    QString reportContent = "Passenger Manifest Report - MABUHAY FLIGHTS\n\n";

    for (const QString& flightId : flightPassengersMap.keys()) {
        QList<QStringList> passengersList = flightPassengersMap[flightId];
        int totalPassengers = 0; // Initialize totalPassengers to 0

        // Calculate the total passengers for this flight
        for (const QStringList& passengerDetails : passengersList) {
            totalPassengers++;
        }

        // Add the flight information to the report content
        reportContent += "Flight Details:\n";
        reportContent += "Flight Number: " + flightId + "\n";
        reportContent += "Destination: " + passengersList.at(0).at(4) + "\n"; // Get the destination from the first passenger

        // Add passenger details to the report content
        int passengerNumber = 1; // Initialize passengerNumber here
        reportContent += "--------------------------------------------------------------\n";
        reportContent += "| No. | Passenger Name    | Contact Number    | Address                        | Payment Method   |\n";
        reportContent += "--------------------------------------------------------------\n";
        for (const QStringList& passengerDetails : passengersList) {
            reportContent += "| " + QString::number(passengerNumber).rightJustified(3, ' ') + " | ";
            reportContent += passengerDetails.at(0).leftJustified(17) + " | ";
            reportContent += passengerDetails.at(2).leftJustified(18) + " | ";
            reportContent += passengerDetails.at(1).leftJustified(30) + " | ";
            reportContent += passengerDetails.at(3).leftJustified(17) + " |\n";
            passengerNumber++;
        }
        reportContent += "--------------------------------------------------------------\n";
        reportContent += "Total Passengers: " + QString::number(totalPassengers) + "\n\n"; // Add the total passengers count
    }

    // Create and open the PassengerManifestReport dialog
    pmr dialog;
    dialog.setReportContent(reportContent);
    dialog.exec();
}

void reports::on_schedule_clicked()
{
    QSqlQuery query;
    QString queryString = "SELECT flight_id, arrival_time, departure_time, flight_duration, destination_id, flight_type "
                          "FROM flights_dom_int";

    if (query.exec(queryString)) {
        // Create and open the ScheduleReport dialog
        schedulereport Schedreport;
        Schedreport.setModal(true);

        // Process each flight's data and generate the report text
        QString reportText = "DOMESTIC FLIGHTS\n\n";

        while (query.next()) {
            QString flightNumber = query.value("flight_id").toString();
            QString arrivalTime = query.value("arrival_time").toString();
            QString departureTime = query.value("departure_time").toString();
            QString flightDuration = query.value("flight_duration").toString();
            QString destination = query.value("destination_id").toString();
            QString flightType = query.value("flight_type").toString();

            if (flightType.toLower() == "domestic") {
                // Generate the report text for each domestic flight
                reportText += "Flight Number: " + flightNumber + "\n";
                reportText += "Arrival: " + arrivalTime + "\n";
                reportText += "Departure: " + departureTime + "\n";
                reportText += "Flight Duration: " + flightDuration + "\n";
                reportText += "Destination: " + destination + "\n";
                reportText += "\n";
            }
        }

        // Add a separator between domestic and international flights
        reportText += "----------------------------------------------------------------------------\n";
        reportText += "INTERNATIONAL FLIGHTS\n\n";

        // Reset the query to process international flights
        query.seek(-1);

        while (query.next()) {
            QString flightNumber = query.value("flight_id").toString();
            QString arrivalTime = query.value("arrival_time").toString();
            QString departureTime = query.value("departure_time").toString();
            QString flightDuration = query.value("flight_duration").toString();
            QString destination = query.value("destination_id").toString();
            QString flightType = query.value("flight_type").toString();

            if (flightType.toLower() == "international") {
                // Generate the report text for each international flight
                reportText += "Flight Number: " + flightNumber + "\n";
                reportText += "Arrival: " + arrivalTime + "\n";
                reportText += "Departure: " + departureTime + "\n";
                reportText += "Flight Duration: " + flightDuration + "\n";
                reportText += "Destination: " + destination + "\n";
                reportText += "\n";
            }
        }

        // Set the final report content and open the dialog
        Schedreport.setReportContent(reportText);
        Schedreport.exec();
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve flight schedule data from the database: " + query.lastError().text());
    }
}


