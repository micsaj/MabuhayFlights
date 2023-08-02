#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookflights.h"
#include "adminlogin.h"
#include "showflights.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BookFlights_clicked();

    void on_admin_clicked();

    void on_ShowFlights_clicked();

    void onDialogOpened();

private:
    Ui::MainWindow *ui;
    void showTime();
    BookFlights *bookflightsDialog;
    AdminLogin *adminloginDialog;
    ShowFlights *showflightsDialog;

};
#endif // MAINWINDOW_H
