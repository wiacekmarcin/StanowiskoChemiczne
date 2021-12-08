#-------------------------------------------------
#
# Project created by QtCreator 2021-11-26T23:56:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StanowiskoChemiczne
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        glowneokno.cpp \
    mainwindow.cpp \
    nowyprojectdlg.cpp \
    oknoczujnikow.cpp \
    oknostatusowe.cpp \
    stanczujnika.cpp \
    sygnalanalogowyform.cpp \
    ustawieniadozownikaokno.cpp \
    ustawieniasygnalowokno.cpp \
    widokczujnika.cpp

HEADERS += \
        glowneokno.h \
    mainwindow.h \
    nowyprojectdlg.h \
    oknoczujnikow.h \
    oknostatusowe.h \
    stanczujnika.h \
    sygnalanalogowyform.h \
    ustawieniadozownikaokno.h \
    ustawieniasygnalowokno.h \
    widokczujnika.h

FORMS += \
        glowneokno.ui \
    mainwindow.ui \
    nowyprojectdlg.ui \
    oknoczujnikow.ui \
    oknostatusowe.ui \
    stanczujnika.ui \
    sygnalanalogowyform.ui \
    ustawieniadozownikaokno.ui \
    ustawieniasygnalowokno.ui \
    widokczujnika.ui

RESOURCES += \
    ikony.qrc

DISTFILES += \
    StanowiskoChemiczne_pl_PL.ts
