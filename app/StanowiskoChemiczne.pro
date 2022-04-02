#-------------------------------------------------
#
# Project created by QtCreator 2021-11-26T23:56:37
#
#-------------------------------------------------

QT       += core gui
QT += multimedia multimediawidgets
#QT += charts
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StanowiskoChemiczne
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += SYMULATOR
DEFINES += DEBUG_DEVICE
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RCC_DIR = ./src
MOC_DIR = ./moc
OBJECTS_DIR = ./objects
INCLUDEPATH = ./include
DESTDIR = ./build
UI_DIR = ./ui
SOURCES += \
    crc8.cpp \
    czujnikanalogowyustawieniawidget.cpp \
    czujnikanalogowywidget.cpp \
    czujnikianalogoweokno.cpp \
    czujnikanalogowyustawieniaokno.cpp \
    digitaloutwidget.cpp \
    dozowniksettings.cpp \
    main.cpp \
    glowneokno.cpp \
    nicards.cpp \
    nowyprojectdlg.cpp \
    oknostatusowe.cpp \
    otwartezawory.cpp \
    serialdevice.cpp \
    serialmessage.cpp \
    stanczujnika.cpp \
    urzadzenia_debug.cpp \
    ustawieniadozownika.cpp \
    ustawieniatestu.cpp \
    videowidget.cpp \
    sygnalanalparam.cpp \
    wejsciaanalogowe.cpp \
    nowytest_1.cpp \
    createtestwizard.cpp \
    nowytestdlg.cpp \
    projectitem.cpp \
    testpage.cpp \
    testpageform.cpp \
    projektwidget.cpp \
    testtabswidget.cpp \
    nowytest_2.cpp \
    nowytest_3.cpp \
    nowytest_4.cpp \
    nowytest_5.cpp \
    nowytest_6.cpp \
    nowytest_7.cpp \
    nowytest_8.cpp \
    disablebutton.cpp \
    dispushbutton.cpp \
    highlowdigitalwidget.cpp \
    urzadzenia.cpp \
    niusb6210.cpp \
    niusb6501.cpp \
    ustawienia.cpp

HEADERS += \
    common.h \
    crc8.h \
    czujnikanalogowyustawieniawidget.h \
    czujnikanalogowywidget.h \
    czujnikianalogoweokno.h \
    czujnikanalogowyustawieniaokno.h \
    digitaloutwidget.h \
    dozowniksettings.h \
    glowneokno.h \
    nicards.h \
    nowyprojectdlg.h \
    oknostatusowe.h \
    otwartezawory.h \
    serialdevice.h \
    serialmessage.h \
    stanczujnika.h \
    urzadzenia_debug.h \
    ustawieniadozownika.h \
    ustawieniatestu.h \
    videowidget.h \
    sygnalanalparam.h \
    wejsciaanalogowe.h \
    nowytest_1.h \
    createtestwizard.h \
    nowytestdlg.h \
    projectitem.h \
    testpage.h \
    testpageform.h \
    projektwidget.h \
    testtabswidget.h \
    nowytest_2.h \
    nowytest_3.h \
    nowytest_4.h \
    nowytest_5.h \
    nowytest_6.h \
    nowytest_7.h \
    nowytest_8.h \
    disablebutton.h \
    dispushbutton.h \
    highlowdigitalwidget.h \
    urzadzenia.h \
    ustawienia.h \
    niusb6210.h \
    niusb6501.h

FORMS += \
    czujnikanalogowyustawieniawidget.ui \
    czujnikanalogowywidget.ui \
    digitaloutwidget.ui \
    dozowniksettings.ui \
    glowneokno.ui \
    nowyprojectdlg.ui \
    oknostatusowe.ui \
    otwartezawory.ui \
    stanczujnika.ui \
    urzadzenia_debug.ui \
    ustawieniadozownika.ui \
    ustawieniatestu.ui \
    videowidget.ui \
    nowytest_1.ui \
    nowytestdlg.ui \
    testpageform.ui \
    projektwidget.ui \
    testtabswidget.ui \
    nowytest_2.ui \
    nowytest_3.ui \
    nowytest_4.ui \
    nowytest_5.ui \
    nowytest_6.ui \
    nowytest_7.ui \
    nowytest_8.ui \
    disablebutton.ui \
    highlowdigitalwidget.ui

RESOURCES += \
    ikony.qrc

DISTFILES += \
    StanowiskoChemiczne_pl_PL.ts \
    lista.txt \
    path

win32 {
     !contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L$$PWD/../NI/DAQmx_ANSI_C_Dev/lib32/msvc/ -lNIDAQmx
    } else {
        LIBS += -L$$PWD/../NI/DAQmx_ANSI_C_Dev/lib64/msvc/ -lNIDAQmx
    }
}



INCLUDEPATH += $$PWD/../NI/DAQmx_ANSI_C_Dev/include
DEPENDPATH += $$PWD/../NI/DAQmx_ANSI_C_Dev/include
