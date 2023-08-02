#include "salesreport.h"
#include "ui_salesreport.h"

salesreport::salesreport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::salesreport)
{
    ui->setupUi(this);
    setStyleSheet("salesreport { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");

}

salesreport::~salesreport()
{
    delete ui;
}


void salesreport::setReportContent(const QString& content)
{
    ui->reportsales->setPlainText(content);
}
