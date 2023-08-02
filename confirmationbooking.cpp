#include "confirmationbooking.h"
#include "ui_confirmationbooking.h"
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QFont>
#include <QFileDialog>


confirmationbooking::confirmationbooking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::confirmationbooking)
{
    ui->setupUi(this);
    setStyleSheet("confirmationbooking { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

}

confirmationbooking::~confirmationbooking()
{
    delete ui;
}

void confirmationbooking::on_download_clicked()
{
    // Retrieve the ticket summary from the flightticket QLabel
    QString ticketSummary = ui->flightticket->text();

    // Create a QTextDocument to hold the ticket content
    QTextDocument ticketDocument;

    // Set up the ticket content using the retrieved ticket summary
    ticketDocument.setPlainText(ticketSummary);

    // Prompt the user to select the save location and filename
    QString filePath = QFileDialog::getSaveFileName(this, "Save Ticket", "mabuhayflights_ticket.pdf", "PDF Files (*.pdf)");

    if (!filePath.isEmpty()) {
        // Create a QPrinter object for generating the PDF
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filePath);

        // Use the QTextDocument to generate the PDF
        ticketDocument.print(&printer);

        QMessageBox::information(this, "Download Successful", "Ticket downloaded successfully.");
    }
}

void confirmationbooking::setTicketSummary(const QString& ticketSummary) {
    // Set the ticket summary text in the flightticket QLabel
    ui->flightticket->setText(ticketSummary);
}
