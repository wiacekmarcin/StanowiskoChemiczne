#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "czujnikanalogowyustawieniaokno.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QVideoWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>

#include "videowidget.h"
#include "nowytest_1.h"
#include "nowytest_2.h"
#include "nowytest_2.h"
#include "createtestwizard.h"
#include "nowytestdlg.h"
#include "projectitem.h"
#include "nowyprojectdlg.h"
#include "testdata.h"
#include "testtabswidget.h"

GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno),
    selectedProject(nullptr),
    selectedTest(nullptr)
{
    ui->setupUi(this);
    changeSelectedTest();

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("Testowy projekt")));
    projekty[qtreewidgetitem] = ProjectItem("Testowy projekt", "Lista czlonkow",
                                            "/home/test", "Komentarz", "Dzisiejsza data");
    selectedProject = qtreewidgetitem;
    ui->treeWidget->setCurrentItem(qtreewidgetitem);

    qtreewidgetitem = new QTreeWidgetItem(selectedProject, QStringList(QString("Nowy test")));


    selectedProject->addChild(qtreewidgetitem);

    selectedTest = qtreewidgetitem;
    TestData testDane("Woda 1mg", 1, "Woda", 1.0, "Iskra mechaniczna", "--");


    testy[selectedTest] = new TestTabsWidget(projekty[selectedProject],
                                            testDane,
                                            ui->testyStackedWidget);
    ui->testyStackedWidget->addWidget(testy[selectedTest]);
    ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);
    testy[selectedTest]->setActive();

    projekty[selectedProject].addTest(testDane);

    ui->treeWidget->setCurrentItem(selectedTest);
    mapTesty[selectedTest] = selectedProject;

    changeSelectedTest();

}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}

void GlowneOkno::on_actionUstawienia_sygna_w_triggered()
{
    CzujnikAnalogowyUstawieniaOkno *dlg = new CzujnikAnalogowyUstawieniaOkno(settings, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        dlg->saveData(settings);
    }
}

void GlowneOkno::on_actionNowy_projekt_triggered()
{
    auto * dlg = new NowyProjectDlg(this);
    if (dlg->exec() == QDialog::Rejected) {
        return;
    }

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(ui->treeWidget, QStringList(dlg->getName()));
    projekty[qtreewidgetitem] = ProjectItem(dlg->getName(), dlg->getMembers(), dlg->getWorkDir(), dlg->getComment(),
                                            dlg->getDate());
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

    selectedTest = qtreewidgetitem;
    TestData testDane(dlg->getName(), dlg->getDozownik(), dlg->getLuquid(),
             dlg->getVolume(), dlg->getIngition(), dlg->getIngitionExt());


    testy[selectedTest] = new TestTabsWidget(projekty[selectedProject],
                                            testDane,
                                            ui->testyStackedWidget);
    ui->testyStackedWidget->addWidget(testy[selectedTest]);
    ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);
    testy[selectedTest]->setActive();

    projekty[selectedProject].addTest(testDane);

    ui->treeWidget->setCurrentItem(selectedTest);
    mapTesty[selectedTest] = selectedProject;
    changeSelectedTest();
}

void GlowneOkno::on_treeWidget_itemClicked(QTreeWidgetItem *item, int/* column */)
{
    if (item == nullptr) {
        selectedProject = nullptr;
        selectedTest = nullptr;
        changeSelectedTest();
        return;
    }

    if (projekty.keys().contains(item)) {
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
    ui->actionDodaj_film->setDisabled(selectedTest == nullptr);
    ui->actionDodaj_zdj_cie->setDisabled(selectedTest == nullptr);
    ui->actionTw_rz_raport->setDisabled(selectedTest == nullptr);
    if (selectedTest) {
        ui->stackedWidget->setVisible(true);
        ui->stackedWidget->setCurrentIndex(1);
        ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);
        testy[selectedTest]->setActive();
    } else if (selectedProject) {
        ui->stackedWidget->setVisible(true);
        ui->stackedWidget->setCurrentIndex(0);
        projekty[selectedProject].setWidget(ui->Projekt);
    } else {
        ui->stackedWidget->setVisible(false);
    }
}
