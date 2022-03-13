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
    qDebug("%s %d",__FILE__,__LINE__);
    initialSetting();
    qDebug("%s %d",__FILE__,__LINE__);
    ui->setupUi(this);
    ui->widget->setParams(settings);
    qDebug("%s %d",__FILE__,__LINE__);
    dlgUrz = new Urzadzenia(this);
    qDebug("%s %d",__FILE__,__LINE__);
    dlgUrz->setLabels(settings);
    qDebug("%s %d",__FILE__,__LINE__);
    dlgUrz->setUstawienia(settings);
    dlgUrz->setUstawienia(&settings);
    qDebug("%s %d",__FILE__,__LINE__);
    dlgUrz->setHidden(true);
    qDebug("%s %d",__FILE__,__LINE__);
    connect(dlgUrz,&Urzadzenia::analogValueChanged, this, &GlowneOkno::valueChanged);
    connect(this, &GlowneOkno::analogValueChanged, ui->widget, &CzujnikiAnalogoweOkno::updateValue);
    connect(dlgUrz, &Urzadzenia::drzwi_komory, ui->frCzujniki, &OknoStatusowe::setDrzwiKomory);
    connect(dlgUrz, &Urzadzenia::zawor, ui->frCzujniki, &OknoStatusowe::setZawor);
    connect(dlgUrz, &Urzadzenia::pilot_btn, ui->frCzujniki, &OknoStatusowe::setPilot);
    connect(dlgUrz, &Urzadzenia::usb6210, ui->frCzujniki, &OknoStatusowe::setUSB6210);
    connect(dlgUrz, &Urzadzenia::usb6501, ui->frCzujniki, &OknoStatusowe::setUSB6501);
    connect(dlgUrz, &Urzadzenia::dozownik, ui->frCzujniki, &OknoStatusowe::setDozownik);
    qDebug("%s %d",__FILE__,__LINE__);
    ui->frCzujniki->setLabels(settings);
    qDebug("%s %d",__FILE__,__LINE__);

    signalMapper = new QSignalMapper(this);

    for (int i = 0; i < settings.maxCzujekAnal; ++i) {
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
        signalMapper->setMapping(act_wyzwal[i],     settings.maxCzujekAnal + i);
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
// testowy test
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
    connect(dlgUrz, &Urzadzenia::digitalValueChanged, testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeDigitalIn);
    connect(this, &GlowneOkno::analogValueChanged, testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeAnalog);
//end
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

    connect(dlgUrz, &Urzadzenia::digitalValueChanged, testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeDigitalIn);
    connect(this, &GlowneOkno::analogValueChanged, testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeAnalog);
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

    if (projekty.contains(item)) {
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
    //qDebug("%d", id);
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
    for (int i = 0; i < settings.maxCzujekAnal; ++i) {
        act_wykresy[i]->setText(settings.getName(i+1));
        act_wyzwal[i]->setText(settings.getName(i+1));
    }
}

void GlowneOkno::initialSetting()
{
    //if (cos) return;

    settings.setCzujka(a_cisn_komora,   QString::fromUtf8("Ci\305\233nenie w komorze"), QString::fromUtf8("kPa"), 1.0);
    settings.setCzujka(a_vol1,          QString::fromUtf8("St\304\231\305\274enie VOC 1"), QString::fromUtf8("%"), 1.0);
    settings.setCzujka(a_o2,            QString::fromUtf8("St\304\231\305\274enie O2"), QString::fromUtf8("%"), 1.0);
    settings.setCzujka(a_co2,           QString::fromUtf8("St\304\231\305\274enie CO2"), QString::fromUtf8("%"), 1.0);
    settings.setCzujka(a_temp_komory,   QString::fromUtf8("Temperatura w komorze"), QString::fromUtf8("\u00B0 C"), 1.0);
    settings.setCzujka(a_temp_parownik, QString::fromUtf8("Temperatura parownika"), QString::fromUtf8("\u00B0 C"), 1.0);
    settings.setCzujka(a_vol2,          QString::fromUtf8("St\304\231\305\274enie VOC 2"), QString::fromUtf8("mV"), 1.0);
    settings.setCzujka(a_8,             QString::fromUtf8("Syg. analogowy 8"), QString::fromUtf8("mV"), 1.0);

    settings.setWejscie(drzwi_lewe,         QString::fromUtf8("Kontaktron drzwi lewe komory"));
    settings.setWejscie(drzwi_prawe,        QString::fromUtf8("Kontaktron drzwi lewe komory"));
    settings.setWejscie(wentylacja_lewa,    QString::fromUtf8("Wentylacja lewa"));
    settings.setWejscie(wentylacja_prawa,   QString::fromUtf8("Wentylacja prawa"));
    settings.setWejscie(pom_stez_1,         QString::fromUtf8("Pomiar st\304\231\305\274enia 1"));
    settings.setWejscie(pom_stez_2,         QString::fromUtf8("Pomiar st\304\231\305\274enia 2"));
    settings.setWejscie(wlot_powietrza,     QString::fromUtf8("Powietrze"));
    settings.setWejscie(proznia,            QString::fromUtf8("Pr\303\263\305\272nia"));
    settings.setWejscie(pilot,              QString::fromUtf8("Pilot zdalnego sterowania"));

    settings.setWyjscie(hv_onoff,           QString::fromUtf8("Iskra elektryczna ON/OFF"));
    settings.setWyjscie(hv_zaplon           QString::fromUtf8("Iskra elektryczna High Voltage"));
    settings.setWyjscie(hw_iskra,           QString::fromUtf8("Iskra elektryczna Zap\305\202on"));
    settings.setWyjscie(iskra_mechaniczna, QString::fromUtf8("Iskra mechaniczna (ON/OFF silnik DC)"));
    settings.setWyjscie(grzalka_onoff, QString::fromUtf8("P\305\202omie\305\204 (ON/OFF grza\305\202ki)"));
    settings.setWyjscie(pompa_prozniowa, QString::fromUtf8("Pompa pr\303\263\305\274niowa"));
    settings.setWyjscie(pompa_mebramowa, QString::fromUtf8("Pompka mebramowa"));
    settings.setWyjscie(wentylator, QString::fromUtf8("Wentylator do przedmuchu"));
    settings.setWyjscie(mieszadlo, QString::fromUtf8("Mieszad\305\202o"));
    settings.setWyjscie(kamera, QString::fromUtf8("Trigger kamery"));
}

