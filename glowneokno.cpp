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
#include <QSignalMapper>


#include "videowidget.h"
#include "createtestwizard.h"
#include "nowytestdlg.h"
#include "projectitem.h"
#include "nowyprojectdlg.h"
#include "testdata.h"
#include "testtabswidget.h"
#include "urzadzenia.h"

GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno),
    selectedProject(nullptr),
    selectedTest(nullptr)
{
    initialSetting();
    ui->setupUi(this);
    ui->widget->setParams(settings);
    dlgUrz = new Urzadzenia(this);
    dlgUrz->setLabels(settings);
    dlgUrz->setHidden(true);
    connect(dlgUrz,&Urzadzenia::analogValueChanged, this, &GlowneOkno::valueChanged);

    connect(this, &GlowneOkno::analogValueChanged, ui->widget, &CzujnikiAnalogoweOkno::updateValue);


    signalMapper = new QSignalMapper(this);

    for (int i = 0; i < settings.maxCzujek; ++i) {
        act_wykresy[i] = new QAction(this);
        act_wykresy[i]->setObjectName(QString("actionCzujnikChart_%1").arg(i));
        act_wykresy[i]->setCheckable(true);
        act_wykresy[i]->setText(settings.getName(i+1));
        connect(act_wykresy[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(act_wykresy[i],     i);
        //connect(act_wykresy[i], &QAction::triggered, [this, i] { wybierzCzujke(i); });

        act_wyzwal[i] = new QAction(this);
        act_wyzwal[i]->setObjectName(QString("actionCzujnikTrigger_%1").arg(i));
        act_wyzwal[i]->setCheckable(true);
        act_wyzwal[i]->setText(settings.getName(i+1));
        connect(act_wyzwal[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(act_wyzwal[i],     settings.maxCzujek + i);
        //connect(act_wyzwal[i], &QAction::triggered, [this, i] { wybierzCzujke(settings.maxCzujek+i); });

        ui->menuPodgl_d_wej->addAction(act_wykresy[i]);
        ui->menuWyzwalanie_wyj->addAction(act_wyzwal[i]);
    }

    connect (signalMapper, SIGNAL( mapped(int) ), this, SLOT(wybierzCzujke(int))) ;

    QAction * debugAct = new QAction(this);
    debugAct->setObjectName("debugaction");
    debugAct->setText(QString::fromUtf8("Symulator wej\305\233\304\207/wyj\305\233\304\207"));
    ui->menuBar->addAction(debugAct);
    connect(debugAct, &QAction::triggered, this, &GlowneOkno::showIO);
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
        ui->widget->setParams(settings);
        setActionText();
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

void GlowneOkno::wybierzCzujke(int id)
{
    qDebug("%d", id);
}

void GlowneOkno::showIO()
{
    //showDebugDlg = !showDebugDlg;
    //if (showDebugDlg)
    //    dlgUrz->show();
    //else
    //    dlgUrz->hide();
    dlgUrz->show();
}

void GlowneOkno::valueChanged(int id, int mv)
{
    double valratio = settings.getRatio(id) * mv;
    emit analogValueChanged(id, valratio);

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

void GlowneOkno::setActionText()
{
    for (int i = 0; i < settings.maxCzujek; ++i) {
        act_wykresy[i]->setText(settings.getName(i+1));
        act_wyzwal[i]->setText(settings.getName(i+1));
    }
}

void GlowneOkno::initialSetting()
{
    //if (cos) return;
    settings.setZawor(1, QString::fromUtf8("Ci\305\233nenie w komorze"), QString::fromUtf8("kPa"), 1.0);
    settings.setZawor(2, QString::fromUtf8("St\304\231\305\274enie VOC"), QString::fromUtf8("%"), 1.0);
    settings.setZawor(3, QString::fromUtf8("St\304\231\305\274enie O2"), QString::fromUtf8("%"), 1.0);
    settings.setZawor(4, QString::fromUtf8("St\304\231\305\274enie CO2"), QString::fromUtf8("%"), 1.0);
    settings.setZawor(5, QString::fromUtf8("Temperatura w komorze"), QString::fromUtf8("\u00B0 C"), 1.0);
    settings.setZawor(6, QString::fromUtf8("Temperatura parownika"), QString::fromUtf8("\u00B0 C"), 1.0);
    settings.setZawor(7, QString::fromUtf8("Sygnał analogowy 7"), QString::fromUtf8("mV"), 1.0);
    settings.setZawor(8, QString::fromUtf8("Sygnał analogowy 8"), QString::fromUtf8("mV"), 1.0);
}

