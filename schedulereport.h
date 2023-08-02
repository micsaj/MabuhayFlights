#ifndef SCHEDULEREPORT_H
#define SCHEDULEREPORT_H

#include <QDialog>

namespace Ui {
class schedulereport;
}

class schedulereport : public QDialog
{
    Q_OBJECT

public:
    explicit schedulereport(QWidget *parent = nullptr);
    ~schedulereport();

    void setReportContent(const QString& content);

private:
    Ui::schedulereport *ui;
};

#endif // SCHEDULEREPORT_H
