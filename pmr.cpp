#include "pmr.h"
#include "ui_pmr.h"

pmr::pmr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pmr)
{
    ui->setupUi(this);
    setStyleSheet("reports { background-image: url(:/pix/MABUHAY FLIGHTS/bg); }");
}

pmr::~pmr()
{
    delete ui;
}

void pmr::setReportContent(const QString& content)
{
    ui->reportText->setPlainText(content);
}
