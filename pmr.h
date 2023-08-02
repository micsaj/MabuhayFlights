#ifndef PMR_H
#define PMR_H

#include <QDialog>

namespace Ui {
class pmr;
}

class pmr : public QDialog
{
    Q_OBJECT

public:
    explicit pmr(QWidget *parent = nullptr);
    ~pmr();

    void setReportContent(const QString& content);

private:
    Ui::pmr *ui;
};

#endif // PMR_H
