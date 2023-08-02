#include "schedulereport.h"
#include "ui_schedulereport.h"

schedulereport::schedulereport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::schedulereport)
{
    ui->setupUi(this);
    setStyleSheet("schedulereport { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

}

schedulereport::~schedulereport()
{
    delete ui;
}


void schedulereport::setReportContent(const QString& content)
{
    ui->schedule_report->setPlainText(content);
}
