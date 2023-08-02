 #include "bookflights.h"
#include "ui_bookflights.h"
#include "confirmationbooking.h"

#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVBoxLayout>
#include <QSqlQuery>

BookFlights::BookFlights(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookFlights)
{
    ui->setupUi(this);
       setStyleSheet("BookFlights { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");



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

       // Set up the QCalendarWidget for departure date
       QCalendarWidget* calendarDeparture = new QCalendarWidget(this);
       calendarDeparture->setStyleSheet("background-color: white; color: rgb(0, 85, 127);");
       calendarDeparture->setMinimumDate(QDate::currentDate());
       calendarDeparture->setMaximumDate(QDate(2023, 12, 31)); // Adjust the maximum date as needed
       ui->departureDE->setCalendarWidget(calendarDeparture);
       ui->departureDE->setDate(QDate::currentDate());
       ui->departureDE->setCalendarPopup(true);
       calendarDeparture->setGeometry(ui->departureDE->geometry()); // Set the geometry of the calendar widget to match the QDateEdit field

       // Connect the dateChanged signal of departureDE to a slot that updates the return date
       connect(ui->departureDE, SIGNAL(dateChanged(QDate)), this, SLOT(updateReturnDate(QDate)));

       // Set the minimum date of departureDE to the current date
       ui->departureDE->setMinimumDate(QDate::currentDate());
}

BookFlights::~BookFlights()
{
    delete ui;
}

 void BookFlights::on_home_clicked()
{
    accept();
}


 int BookFlights::getTicketsSoldForFlight(const QString& flightNumber)
 {
     // Prepare the query to fetch the tickets_sold for the given flightNumber
     QSqlQuery query;
     QString queryStr = "SELECT tickets_sold FROM ticket_sales WHERE flight_id = :flightNo";
     query.prepare(queryStr);
     query.bindValue(":flightNo", flightNumber);

     // Execute the query and fetch the results
     if (query.exec() && query.next())
     {
         // Assuming the tickets_sold column is an integer
         int ticketsSold = query.value(0).toInt();
         return ticketsSold;
     }
     else
     {
         // Handle any errors or return a default value (e.g., 0) if the flight is not found
         // You may also show an error message if needed.
         qDebug() << "Failed to fetch tickets_sold for flight: " << flightNumber;
         return 0;
     }
 }


 void BookFlights::on_checkout_clicked()
 {
     // Retrieve the passenger information
     QString name = ui->name->text();
     QString address = ui->address->text();
     QString contactNo = ui->contact->text();

     // Retrieve the selected payment method
     QString paymentMethod;
     if (ui->gcash->isChecked())
         paymentMethod = "GCash";
     else if (ui->creditcard->isChecked())
         paymentMethod = "Credit Card";

     // Retrieve the destination from the QLabel
     QString destination = ui->destination->text(); // Assuming the QLabel is named "destination"

     // Validate the input fields
     if (name.isEmpty() || address.isEmpty() || contactNo.isEmpty())
     {
         QMessageBox messageBox;
         messageBox.setStyleSheet("background-color: rgb(0, 85, 127); color: white;");
         messageBox.setWindowTitle("Incomplete Details");
         messageBox.setText("Please fill in all the required fields.");
         messageBox.exec();
         return;
     }

     if (paymentMethod.isEmpty())
     {
         QMessageBox messageBox;
         messageBox.setStyleSheet("background-color: rgb(0, 85, 127); color: white;");
         messageBox.setWindowTitle("Payment Method");
         messageBox.setText("Please select a payment method.");
         messageBox.exec();
         return;
     }


     // Check if the flight is fully booked
         QString flightNumber = ui->flightno->text();
         QSqlQuery checkFullyBookedQuery;
         QString checkFullyBookedQueryStr = "SELECT available_seats FROM flights_dom_int WHERE flight_id = :flightNo";
         checkFullyBookedQuery.prepare(checkFullyBookedQueryStr);
         checkFullyBookedQuery.bindValue(":flightNo", flightNumber);

         if (checkFullyBookedQuery.exec() && checkFullyBookedQuery.next())
         {
             int availableSeats = checkFullyBookedQuery.value("available_seats").toInt();
             if (availableSeats == 0)
             {
                 // If available seats are 0, show a message and stop the booking process
                 QMessageBox::StandardButton reply = QMessageBox::warning(this, "Flight Fully Booked", "This flight is now fully booked.", QMessageBox::Ok);
                 if (reply == QMessageBox::Ok)
                 {
                     // If the flight is fully booked, stop the booking process
                     return;
                 }
             }
             else if (availableSeats > 0) // Make sure availableSeats is positive before updating
             {
                 // Update the available_seats value in the database (decrement by 1)
                 QSqlQuery updateAvailableSeatsQuery;
                 QString updateAvailableSeatsQueryStr = "UPDATE flights_dom_int SET available_seats = :availableSeats WHERE flight_id = :flightNo";
                 updateAvailableSeatsQuery.prepare(updateAvailableSeatsQueryStr);
                 updateAvailableSeatsQuery.bindValue(":flightNo", flightNumber);
                 updateAvailableSeatsQuery.bindValue(":availableSeats", availableSeats - 1); // Decrement availableSeats

                 if (!updateAvailableSeatsQuery.exec())
                 {
                     QMessageBox::critical(this, "Database Error", "Failed to update available seats in the database: " + updateAvailableSeatsQuery.lastError().text());
                     return;
                 }
             }
         }
         else
         {
             QMessageBox::critical(this, "Database Error", "Failed to check available seats in the database: " + checkFullyBookedQuery.lastError().text());
             return;
         }


     // Store the passenger information in the database
     QSqlQuery insertQuery;
     QString insertQueryStr = "INSERT INTO passenger_info (name, address, payment_method, contactno, flight_id, destination) "
                              "VALUES (:name, :address, :paymentMethod, :contactNo, :flightNo, :destination)";
     insertQuery.prepare(insertQueryStr);
     insertQuery.bindValue(":name", name);
     insertQuery.bindValue(":address", address);
     insertQuery.bindValue(":paymentMethod", paymentMethod);
     insertQuery.bindValue(":contactNo", contactNo);
     insertQuery.bindValue(":flightNo", ui->flightno->text());
     insertQuery.bindValue(":destination", destination); // Store the destination in the database

     if (!insertQuery.exec())
     {
         QMessageBox::critical(this, "Database Error", "Failed to store passenger information in the database: " + insertQuery.lastError().text());
         return;
     }


     // Increment the 'tickets_sold' column in the ticket_sales table
     QSqlQuery updateTicketsSoldQuery;
     QString updateTicketsSoldQueryStr = "UPDATE ticket_sales SET tickets_sold = tickets_sold + 1 WHERE flight_id = :flightNo";
     updateTicketsSoldQuery.prepare(updateTicketsSoldQueryStr);
     updateTicketsSoldQuery.bindValue(":flightNo", flightNumber);

     if (!updateTicketsSoldQuery.exec())
     {
         QMessageBox::critical(this, "Database Error", "Failed to update tickets_sold in the database: " + updateTicketsSoldQuery.lastError().text());
         return;
     }

     // Fetch the 'ticket_price' for the selected flight_id from the ticket_sales table
     double ticketPrice = 0.0;
     QSqlQuery fetchTicketPriceQuery;
     QString fetchTicketPriceQueryStr = "SELECT ticket_price FROM ticket_sales WHERE flight_id = :flightNo";
     fetchTicketPriceQuery.prepare(fetchTicketPriceQueryStr);
     fetchTicketPriceQuery.bindValue(":flightNo", flightNumber);

     if (fetchTicketPriceQuery.exec() && fetchTicketPriceQuery.next())
     {
         QString ticketPriceStr = fetchTicketPriceQuery.value(0).toString();
         // Remove commas from the ticketPriceStr before converting to double
         ticketPriceStr.remove(',');
         ticketPrice = ticketPriceStr.toDouble();
     }
     else
     {
         QMessageBox::critical(this, "Database Error", "Failed to fetch ticket_price from the database: " + fetchTicketPriceQuery.lastError().text());
         return;
     }

     // Calculate the revenuePrice based on the number of tickets sold and travelClass
     int ticketsSold = getTicketsSoldForFlight(flightNumber); // Function to retrieve the current tickets_sold for the flight_id
     double revenuePrice = ticketPrice * ticketsSold;

     QString travelClass = ui->travelclass->text();
     if (travelClass == "Business" || travelClass == "B")
     {
         revenuePrice *= 2.5; // Multiply the revenuePrice by 2.5 for Business class
     }

     // Update the 'revenue' column in the ticket_sales table with the calculated revenuePrice
     QSqlQuery updateRevenueQuery;
     QString updateRevenueQueryStr = "UPDATE ticket_sales SET revenue = :revenuePrice WHERE flight_id = :flightNo";
     updateRevenueQuery.prepare(updateRevenueQueryStr);
     updateRevenueQuery.bindValue(":revenuePrice", revenuePrice);
     updateRevenueQuery.bindValue(":flightNo", flightNumber);

     if (!updateRevenueQuery.exec())
     {
         QMessageBox::critical(this, "Database Error", "Failed to update revenue in the database: " + updateRevenueQuery.lastError().text());
         return;
     }

     // Update booked seats in the flight_occupancy table (increment by 1)
     QSqlQuery updateBookedSeatsQuery;
     QString updateBookedSeatsQueryStr = "UPDATE flight_occupancy SET booked_seats = booked_seats + 1 WHERE flight_id = :flightNo";
     updateBookedSeatsQuery.prepare(updateBookedSeatsQueryStr);
     updateBookedSeatsQuery.bindValue(":flightNo", flightNumber);

     if (!updateBookedSeatsQuery.exec())
     {
         QMessageBox::critical(this, "Database Error", "Failed to update booked seats in the database: " + updateBookedSeatsQuery.lastError().text());
         return;
     }

     // Calculate the available seats for the flight
     QSqlQuery updateAvailableSeatsQuery;
     QString updateAvailableSeatsQueryStr = "UPDATE flight_occupancy SET availableseats = total_seats - booked_seats WHERE flight_id = :flightNo";
     updateAvailableSeatsQuery.prepare(updateAvailableSeatsQueryStr);
     updateAvailableSeatsQuery.bindValue(":flightNo", flightNumber);

     if (!updateAvailableSeatsQuery.exec())
     {
         QMessageBox::critical(this, "Database Error", "Failed to update available seats in the database: " + updateAvailableSeatsQuery.lastError().text());
         return;
     }


     // Retrieve the selected flight information
     QString flightNo = ui->flightno->text();
     QString departFrom = ui->departfrom->text();
     QString depDate = ui->depdate->text();
     QString depTime = ui->deptime->text();
     QString arrivalTime = ui->arrivaltime->text();
     QString flightDuration = ui->flightduration->text();
     QString flightType = ui->flighttype->text();

     // Validate the input fields
     if (flightNo.isEmpty() || departFrom.isEmpty() || destination.isEmpty() ||
     depDate.isEmpty() || depTime.isEmpty() || arrivalTime.isEmpty() ||
     flightDuration.isEmpty() || travelClass.isEmpty() || flightType.isEmpty() ||
     name.isEmpty() || address.isEmpty() || contactNo.isEmpty()) {
     QMessageBox::warning(this, "Incomplete Information", "Please fill in all the required fields.");
     return;
     }

     // Retrieve the current date and time
     QDateTime ticketIssued = QDateTime::currentDateTime();

     // Generate the ticket summary
     QString ticketSummary = "Flight No: " + flightNo + "\n" +
     "Departure From: " + departFrom + "\n" +
     "Destination: " + destination + "\n" +
     "Departure Date: " + depDate + "\n" +
     "Departure Time: " + depTime + "\n" +
     "Arrival Time: " + arrivalTime + "\n" +
     "Flight Duration: " + flightDuration + "\n" +
     "Travel Class: " + travelClass + "\n" +
     "Flight Type: " + flightType + "\n" +
     "Name: " + name + "\n" +
     "Address: " + address + "\n" +
     "Contact: " + contactNo + "\n" +
     "Ticket Issued: " + ticketIssued.toString();

     // Pass the ticket summary to the ConfirmationBooking dialog
     confirmationbooking Confirm;
     Confirm.setModal(true);
     Confirm.setTicketSummary(ticketSummary);
     Confirm.exec();
 }




void BookFlights::on_domestic_clicked()
{
    ui->comboBox_destination->clear();
    ui->comboBox_destination->addItem("CEBU (CEB)");
    ui->comboBox_destination->addItem("DAVAO (DVO)");
    ui->comboBox_destination->addItem("PUERTO PRINSESA (PPS)");


    ui->flighttype->setText("Domestic");
}

void BookFlights::on_international_clicked()
{
    ui->comboBox_destination->clear();
    ui->comboBox_destination->addItem("SAPPORO (JPN)");
    ui->comboBox_destination->addItem("INCHEON (KOR)");
    ui->comboBox_destination->addItem("ARENDELLE (NOR)");

    ui->flighttype->setText("International");
}

void BookFlights::on_searchflight_clicked()
{
    // Retrieve the selected values from the UI
    QString destination = ui->comboBox_destination->currentText();
    ui->destination->setText(destination);

    // Retrieve flight time option
    QString timeOption;
    if (ui->earlyflight->isChecked())
        timeOption = "Early";
    else if (ui->morningflight->isChecked())
        timeOption = "Morning";
    else if (ui->afternoon->isChecked())
        timeOption = "Afternoon";
    else if (ui->nightflight->isChecked())
        timeOption = "Night";

    // Retrieve flight type option
    QString flightType;
    if (ui->domestic->isChecked())
        flightType = "Domestic";
    else if (ui->international->isChecked())
        flightType = "International";

    // Retrieve travel class option
    QString travelClass;
    if (ui->economy->isChecked())
        travelClass = "Economy";
    else if (ui->business->isChecked())
        travelClass = "Business";



    // Declare the query variable
    QSqlQuery query;

    // Retrieve data from the flights table based on the selected options
    QString queryString = "SELECT flight_id, arrival_time, departure_time, available_seats, total_cost, flight_duration "
                          "FROM flights_dom_int "
                          "WHERE flight_type = :flightType AND flight_time = :timeOption AND destination_id = :destination";
    query.prepare(queryString);
    query.bindValue(":flightType", flightType);
    query.bindValue(":timeOption", timeOption);
    query.bindValue(":destination", destination);

    qDebug() << "SQL query: " << query.lastQuery(); // Print the SQL query being executed

    if (!query.exec()) {
        QMessageBox messageBox;
        messageBox.setStyleSheet("background-color: white; color: black;");
        messageBox.setWindowTitle("Database Error");
        messageBox.setText("Failed to retrieve flight information from the database: " + query.lastError().text());
        messageBox.exec();
        return;
    }

    // Check if the query returned any result
    if (query.next()) {
        // Retrieve the data from the query result
        QString flightNo = query.value("flight_id").toString();
        QString arrivalTime = query.value("arrival_time").toString();
        QString departureTime = query.value("departure_time").toString();
        int availableSeats = query.value("available_seats").toInt();
        QString totalCostStr = query.value("total_cost").toString();
        QString flightDuration = query.value("flight_duration").toString();
        totalCostStr.remove(','); // Remove commas from the string

        double totalCost = totalCostStr.toDouble();

        // Apply modifications if travel class is "Business"
        if (travelClass == "Business") {
            totalCost *= 2.5;
        }




        // Set the retrieved data in the corresponding QLabel widgets
        ui->flightno->setText(flightNo);
        ui->arrivaltime->setText(arrivalTime);
        ui->deptime->setText(departureTime);
        ui->availableseats->setText(QString::number(availableSeats));
        ui->flightduration->setText(flightDuration);

        // Convert the total cost to a string with the appropriate precision
        QString totalCostFormatted = QString::number(totalCost, 'f', 2) + " PHP";
        ui->totalcost->setText(totalCostFormatted);

        // Check if the flight is fully booked
               if (availableSeats == 0) {
                   QMessageBox messageBox;
                   messageBox.setStyleSheet("background-color: white; color: black;");
                   messageBox.setWindowTitle("Flight Fully Booked");
                   messageBox.setText("Selected flight is fully booked. Please choose another flight departure");
                   messageBox.exec();
                   return;
               }


    } else {
        QMessageBox messageBox;
        messageBox.setStyleSheet("background-color: white; color: black;");
        messageBox.setWindowTitle("No Flights Found");
        messageBox.setText("No flights matching the selected options were found.");
        messageBox.exec();
        return;
    }


    qDebug() << "Selected Flight Type:" << flightType;
    qDebug() << "Selected Flight Time:" << timeOption;
    qDebug() << "Selected Destination:" << destination;

    // Display the selected departure date in the depdate QLabel
    QDate departureDate = ui->departureDE->date();
    QString departureDateString = departureDate.toString("MM-dd-yyyy");
    ui->depdate->setText(departureDateString);
    ui->departfrom->setText("Manila (MNL)");

    // Update the return date QLabel based on the flight trip option
    if (ui->flighttrip->text() == "Round-Trip") {
        QDate returnDate = departureDate.addDays(7); // Calculate the return date as 1 week after departure
        QString returnDateString = returnDate.toString("MM-dd-yyyy");
        ui->returndate->setText(returnDateString); // Update the return date QLabel
    } else {
        ui->returndate->clear(); // Clear the return date QLabel
        ui->returndate->setText(""); // Set the return date QLabel to empty string
    }



    // Rest of the code...
}


void BookFlights::on_oneway_clicked()
{
    ui->flighttrip->setText("One-Way");
    ui->returndate->clear(); // Clear the return date QLabel
    ui->returndate->setText("");
}

void BookFlights::on_roundtrip_clicked()
{
    ui->flighttrip->setText("Round-Trip");


}

void BookFlights::on_economy_clicked()
{
    ui->travelclass->setText("E");
}

void BookFlights::on_business_clicked()
{
    ui->travelclass->setText("B");
}

void BookFlights::on_reset_clicked()
{
    // Reset the UI to initial state
    ui->comboBox_destination->clear();
    ui->flighttrip->setText("");
    ui->departureDE->setDate(QDate::currentDate());
    ui->departfrom->setText("");
    ui->depdate->setText("");
    ui->returndate->setText("");
    ui->destination->setText("");
    ui->flighttype->setText("");
    ui->travelclass->setText("");
    ui->deptime->setText("");
    ui->flightno->setText("");
    ui->deptime->setText("");
    ui->arrivaltime->setText("");
    ui->availableseats->setText("");
    ui->totalcost->setText("");
    ui->flightduration->setText("");


    //Reset radiobuttons to unchecked

    ui->domestic->setAutoExclusive(false);
    ui->domestic->setChecked(false);
    ui->domestic->setAutoExclusive(true);

    ui->international->setAutoExclusive(false);
    ui->international->setChecked(false);
    ui->international->setAutoExclusive(true);

    ui->earlyflight->setAutoExclusive(false);
    ui->earlyflight->setChecked(false);
    ui->earlyflight->setAutoExclusive(true);

    ui->morningflight->setAutoExclusive(false);
    ui->morningflight->setChecked(false);
    ui->morningflight->setAutoExclusive(true);

    ui->afternoon->setAutoExclusive(false);
    ui->afternoon->setChecked(false);
    ui->afternoon->setAutoExclusive(true);

    ui->nightflight->setAutoExclusive(false);
    ui->nightflight->setChecked(false);
    ui->nightflight->setAutoExclusive(true);

    ui->oneway->setAutoExclusive(false);
    ui->oneway->setChecked(false);
    ui->oneway->setAutoExclusive(true);

    ui->roundtrip->setAutoExclusive(false);
    ui->roundtrip->setChecked(false);
    ui->roundtrip->setAutoExclusive(true);

    ui->economy->setAutoExclusive(false);
    ui->economy->setChecked(false);
    ui->economy->setAutoExclusive(true);

    ui->business->setAutoExclusive(false);
    ui->business->setChecked(false);
    ui->business->setAutoExclusive(true);


}


void BookFlights::on_clear_clicked()
{
    ui->name->setText("");
    ui->address->setText("");
    ui->contact->setText("");

    ui->gcash->setAutoExclusive(false);
    ui->gcash->setChecked(false);
    ui->gcash->setAutoExclusive(true);

    ui->creditcard->setAutoExclusive(false);
    ui->creditcard->setChecked(false);
    ui->creditcard->setAutoExclusive(true);


}

