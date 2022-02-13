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
    src/crc8.cpp \
    src/czujnikanalogowyustawieniawidget.cpp \
    src/czujnikanalogowywidget.cpp \
    src/czujnikianalogoweokno.cpp \
    src/czujnikanalogowyustawieniaokno.cpp \
    src/digitaloutwidget.cpp \
    src/main.cpp \
    src/glowneokno.cpp \
    src/nowyprojectdlg.cpp \
    src/oknostatusowe.cpp \
    src/otwartezawory.cpp \
    src/serialmessage.cpp \
    src/stanczujnika.cpp \
    src/videowidget.cpp \
    src/ustawieniadozownikaokno.cpp \
    src/sygnalanalparam.cpp \
    src/wejsciaanalogowe.cpp \
    src/nowytest_1.cpp \
    src/createtestwizard.cpp \
    src/nowytestdlg.cpp \
    src/projectitem.cpp \
    src/testdata.cpp \
    src/testpage.cpp \
    src/testpageform.cpp \
    src/projektwidget.cpp \
    src/testtabswidget.cpp \
    src/nowytest_2.cpp \
    src/nowytest_3.cpp \
    src/nowytest_4.cpp \
    src/nowytest_5.cpp \
    src/nowytest_6.cpp \
    src/nowytest_7.cpp \
    src/nowytest_8.cpp \
    src/disablebutton.cpp \
    src/dispushbutton.cpp \
    src/highlowdigitalwidget.cpp \
    src/urzadzenia.cpp \
    src/ustawienia.cpp

HEADERS += \
    include/crc8.h \
    include/czujnikanalogowyustawieniawidget.h \
    include/czujnikanalogowywidget.h \
    include/czujnikianalogoweokno.h \
    include/czujnikanalogowyustawieniaokno.h \
    include/digitaloutwidget.h \
    include/glowneokno.h \
    include/nowyprojectdlg.h \
    include/oknostatusowe.h \
    include/otwartezawory.h \
    include/serialmessage.h \
    include/stanczujnika.h \
    include/videowidget.h \
    include/ustawieniadozownikaokno.h \
    include/sygnalanalparam.h \
    include/wejsciaanalogowe.h \
    include/nowytest_1.h \
    include/createtestwizard.h \
    include/nowytestdlg.h \
    include/projectitem.h \
    include/testdata.h \
    include/testpage.h \
    include/testpageform.h \
    include/projektwidget.h \
    include/testtabswidget.h \
    include/nowytest_2.h \
    include/nowytest_3.h \
    include/nowytest_4.h \
    include/nowytest_5.h \
    include/nowytest_6.h \
    include/nowytest_7.h \
    include/nowytest_8.h \
    include/disablebutton.h \
    include/dispushbutton.h \
    include/highlowdigitalwidget.h \
    include/urzadzenia.h \
    include/ustawienia.h

FORMS += \
    form/czujnikanalogowyustawieniawidget.ui \
    form/czujnikanalogowywidget.ui \
    form/digitaloutwidget.ui \
    form/glowneokno.ui \
    form/nowyprojectdlg.ui \
    form/oknostatusowe.ui \
    form/otwartezawory.ui \
    form/stanczujnika.ui \
    form/videowidget.ui \
    form/ustawieniadozownikaokno.ui \
    form/nowytest_1.ui \
    form/nowytestdlg.ui \
    form/testpageform.ui \
    form/projektwidget.ui \
    form/testtabswidget.ui \
    form/nowytest_2.ui \
    form/nowytest_3.ui \
    form/nowytest_4.ui \
    form/nowytest_5.ui \
    form/nowytest_6.ui \
    form/nowytest_7.ui \
    form/nowytest_8.ui \
    form/disablebutton.ui \
    form/highlowdigitalwidget.ui \
    form/urzadzenia.ui

RESOURCES += \
    ikony.qrc

DISTFILES += \
    StanowiskoChemiczne_pl_PL.ts \
    path

win32: LIBS += -L$$PWD/../NI/DAQmx_ANSI_C_Dev/lib/msvc/ -lNIDAQmx

INCLUDEPATH += $$PWD/../NI/DAQmx_ANSI_C_Dev/include
DEPENDPATH += $$PWD/../NI/DAQmx_ANSI_C_Dev/include
