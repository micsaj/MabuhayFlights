#ifndef SALESREPORT_H
#define SALESREPORT_H

#include <QDialog>

namespace Ui {
class salesreport;
}

class salesreport : public QDialog
{
    Q_OBJECT

public:
    explicit salesreport(QWidget *parent = nullptr);
    ~salesreport();

    void setReportContent(const QString& content);

private:
    Ui::salesreport *ui;
};

#endif // SALESREPORT_H
