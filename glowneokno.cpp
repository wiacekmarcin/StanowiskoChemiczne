#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "czujnikanalogowyustawieniaokno.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QVideoWidget>
#include <QTreeWidgetItem>

#include "videowidget.h"
#include "nowytest_1.h"
#include "nowytest_2.h"
#include "nowytest_3.h"
#include "createtestwizard.h"
#include "nowytestdlg.h"
#include "projectitem.h"
#include "nowyprojectdlg.h"
#include "testdata.h"

GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno),
    selectedProject(nullptr),
    selectedTest(nullptr)
{
    ui->setupUi(this);
    changeSelectedTest();

    VideoWidget * videowidget = new VideoWidget(ui->tabFoto);
    ui->verticalLayout_4->addWidget(videowidget);

}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}

void GlowneOkno::on_actionUstawienia_sygna_w_triggered()
{
    CzujnikAnalogowyUstawieniaOkno *dlg = new CzujnikAnalogowyUstawieniaOkno(this);
    dlg->exec();
}

void GlowneOkno::on_actionNowy_projekt_triggered()
{
    auto * dlg = new NowyProjectDlg(this);
    if (dlg->exec() == QDialog::Rejected) {
        return;
    }

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(ui->treeWidget, QStringList(dlg->getName()));
    projekty[qtreewidgetitem] = ProjectItem(dlg->getName(), dlg->getMembers(), dlg->getWorkDir(), dlg->getComment(),
                                            dlg->getComment());
    selectedProject = qtreewidgetitem;
    ui->treeWidget->setCurrentItem(qtreewidgetitem);
    changeSelectedTest();

}

void GlowneOkno::on_actionNowy_Test_triggered()
{

    auto * dlg = new NowyTestDlg(this);
    if (dlg->exec() == QDialog::Rejected) {
        return;
    }

    if (selectedProject == nullptr)
        return;

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(selectedProject, QStringList(dlg->getName()));


    selectedProject->addChild(qtreewidgetitem);

    if (selectedTest)
        testy[selectedTest].setVisible(false);

    selectedTest = qtreewidgetitem;
    testy[selectedTest] = TestData(dlg->getName(), dlg->getDozownik(),
                                  dlg->getLuquid(), dlg->getVolume(),
                                  dlg->getIngition(), dlg->getIngitionExt());
    testy[selectedTest].setVisible(true);
    projekty[selectedProject].addTest(testy[selectedTest]);
    ui->treeWidget->setCurrentItem(selectedTest);
    ui->frTest->layout()->addWidget(testy[selectedTest].createWizard(ui->frTest));

    mapTesty[selectedTest] = selectedProject;
    changeSelectedTest();



    //CreateTestWizard wizard(ui->tabTest);
    //wizard.exec();
    //ui->tabTest->validateCurrentPage();

    //ui->tabObszarRoboczy->setVisible(true);
    //ui->tabObszarRoboczy->setCurrentIndex(1);
    //ui->tabObszarRoboczy->setCurrentIndex(0);


}

void GlowneOkno::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (selectedTest)
        testy[selectedTest].setVisible(false);
    if (item == nullptr) {
        selectedProject = nullptr;
        selectedTest = nullptr;
        changeSelectedTest();
        return;
    }

    if (projekty.keys().contains(item)) {\
        selectedProject = item;
        selectedTest = nullptr;
        changeSelectedTest();
        return;
    }

    for (auto it = mapTesty.begin(); it != mapTesty.end(); ++it) {
        if (it.key() == item) {
            selectedProject = it.value();
            selectedTest = item;
            changeSelectedTest();
            break;
        }
    }
}

void GlowneOkno::changeSelectedTest()
{
    ui->actionNowy_Test->setDisabled(selectedProject == nullptr);
    if (selectedTest) {
        ui->stackedWidget->setVisible(true);
        testy[selectedTest].setVisible(true);
        ui->tabObszarRoboczy->setCurrentIndex(0);
        //ui->tabObszarRoboczy->setVisible(true);
        ui->stackedWidget->setCurrentIndex(1);
    } else if (selectedProject) {
        ui->stackedWidget->setVisible(true);
        //ui->tabObszarRoboczy->setVisible(false);
        ui->stackedWidget->setCurrentIndex(0);
        projekty[selectedProject].setWidget(ui->projekt);
    } else {
        ui->stackedWidget->setVisible(false);
    }
}
