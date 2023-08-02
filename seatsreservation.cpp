#include "seatsreservation.h"
#include "ui_seatsreservation.h"
#include <QRegularExpression>

seatsreservation::seatsreservation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::seatsreservation)
{
    ui->setupUi(this);
    setStyleSheet("QDialog { background-image: url(:/new/prefix1/presentation kineme.jpg); }");

    // Connect all the seat buttons to a single slot
    connectAllSeatButtons();
}

seatsreservation::~seatsreservation()
{
    delete ui;
}

void seatsreservation::connectAllSeatButtons()
{
    // Find all the seat buttons using object names
    QList<QPushButton*> seatButtons = findChildren<QPushButton*>(QRegularExpression("^[a-f][1-5]$"));

    // Connect all the seat buttons to the same slot
    for (QPushButton* button : seatButtons) {
        connect(button, SIGNAL(clicked()), this, SLOT(onSeatButtonClicked()));
    }
}

void seatsreservation::onSeatButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender()); // Get the button that triggered the slot
    if (button) {
        // Seat button is clicked, update its appearance
        button->setStyleSheet("QPushButton { background-color: red; }");
        button->setEnabled(false); // Disable the button to prevent further selection
    }
}

void seatsreservation::on_f5_clicked()
{
    // Perform actions for the F5 seat button
    QPushButton* button = ui->f5; // Assuming f5 is the object name of the F5 seat button
    button->setStyleSheet("QPushButton { background-color: red; }");
    button->setEnabled(false);
    // ... perform other actions as needed
}
