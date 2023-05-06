#ifndef COMMON_H
#define COMMON_H


//#define SYMULATOR 0
#define GUI_DEBUG 0

#define FINISHTEST 0

#define TESTPROJEKT 0

#define SKIP_ZAWORY 0


#define MSGBOX_YES_NO(RET, ICON, TITLE, INFO, BUTTONS, DEF_BUTTON, PARENT) \
    do { QMessageBox messageBox(ICON,\
                                (TITLE),\
                                (INFO),\
                                (BUTTONS),\
                                PARENT);\
        messageBox.setButtonText(QMessageBox::Yes, tr("Tak"));\
        messageBox.setButtonText(QMessageBox::No, tr("Nie"));\
        messageBox.setDefaultButton(DEF_BUTTON);\
        RET = messageBox.exec();\
    } while(false)


#endif // COMMON_H
