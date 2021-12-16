#-------------------------------------------------
#
# Project created by QtCreator 2021-11-26T23:56:37
#
#-------------------------------------------------

QT       += core gui multimedia  multimediawidgets

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
    czujnikanalogowyustawieniaokno.cpp \
    czujnikanalogowyustawieniawidget.cpp \
    czujnikanalogowywidget.cpp \
    czujnikianalogoweokno.cpp \
        main.cpp \
        glowneokno.cpp \
        mainwindow.cpp \
        nowyprojectdlg.cpp \
    nowytest_2.cpp \
    nowytest_3.cpp \
    nowytest_4.cpp \
        oknostatusowe.cpp \
        stanczujnika.cpp \
    videowidget.cpp \
    ustawieniadozownikaokno.cpp \
    sygnalanalparam.cpp \
    wejsciaanalogowe.cpp \
    nowytest_1.cpp \
    formularzdialogbuttonbox.cpp

HEADERS += \
    czujnikanalogowyustawieniaokno.h \
    czujnikanalogowyustawieniawidget.h \
    czujnikanalogowywidget.h \
    czujnikianalogoweokno.h \
        glowneokno.h \
        mainwindow.h \
        nowyprojectdlg.h \
    nowytest_2.h \
    nowytest_3.h \
    nowytest_4.h \
        oknostatusowe.h \
        stanczujnika.h \
    videowidget.h \
    ustawieniadozownikaokno.h \
    sygnalanalparam.h \
    wejsciaanalogowe.h \
    nowytest_1.h \
    formularzdialogbuttonbox.h

FORMS += \
    czujnikanalogowyustawieniaokno.ui \
    czujnikanalogowyustawieniawidget.ui \
    czujnikanalogowywidget.ui \
    czujnikianalogoweokno.ui \
        glowneokno.ui \
        mainwindow.ui \
        nowyprojectdlg.ui \
    nowytest_2.ui \
    nowytest_3.ui \
    nowytest_4.ui \
        oknostatusowe.ui \
        stanczujnika.ui \
    videowidget.ui \
    ustawieniadozownikaokno.ui \
    nowytest_1.ui

RESOURCES += \
    ikony.qrc

DISTFILES += \
    StanowiskoChemiczne_pl_PL.ts
