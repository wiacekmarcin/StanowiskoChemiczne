# CMakeList.txt: projekt CMake dla elementu CMakeProject2, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)
project(StanowiskoChemiczne3)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)


file(GLOB source_files *.c *.cpp *.qrc)
file(GLOB header_files *.h *.hpp *.txt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt5_DIR "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")

find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets SerialPort)
#find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets SerialPort)

#https://www.ni.com/en-us/support/downloads/drivers/download.ni-daqmx.html#346240


# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (StanowiskoChemiczne3 "main.cpp"
    "crc8.cpp" "crc8.h"
    "czujnikanalogowyustawieniawidget.cpp" "czujnikanalogowyustawieniawidget.h" "czujnikanalogowyustawieniawidget.ui"
    "czujnikanalogowywidget.cpp" "czujnikanalogowywidget.h" "czujnikanalogowywidget.ui"
    "czujnikianalogoweokno.cpp" "czujnikianalogoweokno.h"  
    "czujnikanalogowyustawieniaokno.cpp" "czujnikanalogowyustawieniaokno.h" 
    "digitaloutwidget.cpp" "digitaloutwidget.h" "digitaloutwidget.ui"
    "glowneokno.cpp" "glowneokno.h" "glowneokno.ui"
    "nowyprojectdlg.cpp" "nowyprojectdlg.h" "nowyprojectdlg.ui"
    "oknostatusowe.cpp" "oknostatusowe.h" "oknostatusowe.ui"
    "otwartezawory.cpp" "otwartezawory.h" "otwartezawory.ui"
    "serialmessage.cpp" "serialmessage.h" 
    "stanczujnika.cpp" "stanczujnika.h" "stanczujnika.ui"
    "videowidget.cpp" "videowidget.h" "videowidget.ui"
    "ustawieniadozownikaokno.cpp" "ustawieniadozownikaokno.h" "ustawieniadozownikaokno.ui"
    "sygnalanalparam.cpp" "sygnalanalparam.h"
    "wejsciaanalogowe.cpp" "wejsciaanalogowe.h"
    "createtestwizard.cpp" "createtestwizard.h"
    "nowytestdlg.cpp" "nowytestdlg.h" "nowytestdlg.ui"
    "nowytest_1.cpp" "nowytest_1.h" "nowytest_1.ui"
    "nowytest_2.cpp" "nowytest_2.h" "nowytest_2.ui"
    "nowytest_3.cpp" "nowytest_3.h" "nowytest_3.ui"
    "nowytest_4.cpp" "nowytest_4.h" "nowytest_4.ui"
    "nowytest_5.cpp" "nowytest_5.h" "nowytest_5.ui"
    "nowytest_6.cpp" "nowytest_6.h" "nowytest_6.ui"
    "nowytest_7.cpp" "nowytest_7.h" "nowytest_7.ui"
    "nowytest_8.cpp" "nowytest_8.h" "nowytest_8.ui"
    "projectitem.cpp" "projectitem.h"
    "testdata.cpp" "testdata.h"
    "testpage.cpp" "testpage.h" 
    "testpageform.cpp" "testpageform.h" "testpageform.ui" 
    "projektwidget.cpp" "projektwidget.h" "projektwidget.ui"
    "testtabswidget.cpp" "testtabswidget.cpp" "testtabswidget.ui"
    "disablebutton.cpp" "disablebutton.h" "disablebutton.ui"
    "dispushbutton.cpp" "dispushbutton.h"
    "highlowdigitalwidget.cpp" "highlowdigitalwidget.h" "highlowdigitalwidget.ui"
    "urzadzenia.cpp" "urzadzenia.h" "urzadzenia.ui"
    "ustawienia.cpp" "ustawienia.h"
    "niusb6501.cpp" "niusb6501.h"
    "niusb6210.cpp" "niusb6210.h"
    "ikony.qrc"
)    
#set_property(CMakeProject2 "CMakeProject2.cpp" PROPERTY SKIP_AUTOGEN ON)

target_link_libraries(StanowiskoChemiczne3 "C:/Devel/StanowiskoChemiczne/NI/DAQmx_ANSI_C_Dev/lib/msvc/NIDAQmx.lib")
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Core.lib)
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Multimedia.lib)
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5MultimediaWidgets.lib)
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Widgets.lib)
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Network.lib)
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Gui.lib)
#target_link_libraries(StanowiskoChemiczne C:\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5SerialPort.lib)

target_link_libraries(StanowiskoChemiczne3 Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::MultimediaWidgets Qt5::SerialPort)
 

                                                                         

#add_library(NIDAQmx STATIC IMPORTED GLOBAL) # or STATIC instead of SHARED
#set_target_properties(NIDAQmx PROPERTIES
#  IMPORTED_LOCATION "C:/Users/Marcin/source/repos/CMakeProject2/DAQmx_ANSI_C_Dev/lib"
#  INTERFACE_INCLUDE_DIRECTORIES "C:/Users/Marcin/source/repos/CMakeProject2/DAQmx_ANSI_C_Dev/include"
#)
include_directories("C:/Users/Marcin/source/repos/CMakeProject2/DAQmx_ANSI_C_Dev/include")
include_directories("C:/Qt/5.15.2/msvc2019_64/include")

# LINK: command "C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx64\x64\link.exe 
#/nologo CMakeProject2\CMakeFiles\CMakeProject2.dir\CMakeProject2_autogen\mocs_compilation.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\main.cpp.obj CMakeProject2\CMakeFiles\CMakeProject2.dir\crc8.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\czujnikanalogowyustawieniawidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\czujnikanalogowywidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\czujnikianalogoweokno.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\czujnikanalogowyustawieniaokno.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\digitaloutwidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\glowneokno.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowyprojectdlg.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\oknostatusowe.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\otwartezawory.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\serialmessage.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\stanczujnika.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\videowidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\ustawieniadozownikaokno.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\sygnalanalparam.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\wejsciaanalogowe.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\createtestwizard.cpp.obj C
#MakeProject2\CMakeFiles\CMakeProject2.dir\nowytestdlg.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_1.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_2.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_3.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_4.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_5.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_6.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_7.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\nowytest_8.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\projectitem.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\testdata.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\testpage.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\testpageform.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\projektwidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\testtabswidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\disablebutton.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\dispushbutton.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\highlowdigitalwidget.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\urzadzenia.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\ustawienia.cpp.obj 
#CMakeProject2\CMakeFiles\CMakeProject2.dir\CMakeProject2_autogen\EWIEGA46WW\qrc_ikony.cpp.obj
#/out:CMakeProject2\CMakeProject2.exe 
#/implib:CMakeProject2\CMakeProject2.lib 
#/pdb:CMakeProject2\CMakeProject2.pdb 
#/version:0.0 
#/machine:x64 
#/INCREMENTAL:NO 
#/subsystem:console 
#-lNIDAQmx NIDAQmx.lib 
#-LC:\Users\Marcin\source\repos\CMakeProject2\DAQmx_ANSI_C_Dev\lib\msvc 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5MultimediaWidgets.lib 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5SerialPort.lib 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5Widgets.lib 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5Multimedia.lib 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5Gui.lib 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5Network.lib 
#C:\Qt\5.15.2\msvc2019_64\lib\Qt5Core.lib 
#kernel32.lib 
#user32.lib 
#gdi32.lib 
#winspool.lib 
#shell32.lib 
#ole32.lib 
#oleaut32.lib 
#uuid.lib 
#comdlg32.lib 
#advapi32.lib 
#/MANIFEST 
#/MANIFESTFILE:CMakeProject2\CMakeProject2.exe.manifest" failed (exit code 1181) with the following output:
