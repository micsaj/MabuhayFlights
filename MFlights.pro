QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminaccess.cpp \
    adminlogin.cpp \
    bookflights.cpp \
    confirmationbooking.cpp \
    flightoccupancy.cpp \
    flightsummary.cpp \
    main.cpp \
    mainwindow.cpp \
    pmr.cpp \
    salesreport.cpp \
    schedulereport.cpp \
    showflights.cpp \
    summaryreports.cpp

HEADERS += \
    adminaccess.h \
    adminlogin.h \
    bookflights.h \
    confirmationbooking.h \
    flightoccupancy.h \
    flightsummary.h \
    mainwindow.h \
    pmr.h \
    salesreport.h \
    schedulereport.h \
    showflights.h \
    summaryreports.h

FORMS += \
    adminaccess.ui \
    adminlogin.ui \
    bookflights.ui \
    confirmationbooking.ui \
    flightoccupancy.ui \
    flightsummary.ui \
    mainwindow.ui \
    pmr.ui \
    salesreport.ui \
    schedulereport.ui \
    showflights.ui \
    summaryreports.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pix.qrc

