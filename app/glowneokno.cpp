#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "czujnikanalogowyustawieniaokno.h"

//#include <QMediaPlayer>
//#include <QMediaPlaylist>
#include <QFile>
//#include <QVideoWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QResizeEvent>

#include "videowidget.h"
#include "createtestwizard.h"
#include "nowytestdlg.h"
#include "projectitem.h"
#include "nowyprojectdlg.h"

#include "testtabswidget.h"
#include "urzadzenia.h"
#include "dozowniksettings.h"
#include "ustawieniadozownika.h"
#include "ustawienia.h"

GlowneOkno::GlowneOkno(Ustawienia & ust, Urzadzenia * urzadz, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno),
    urzadzenia(urzadz),
    settings(ust),
    selectedProject(nullptr),
    selectedTest(nullptr)
{
    ui->setupUi(this);
    ui->frCzujniki->setLabels(settings);
    ui->widget->setParams(settings);

    connect(urzadzenia, &Urzadzenia::analogValueChanged, this,       &GlowneOkno::valueChanged);
    connect(urzadzenia, &Urzadzenia::analogValueChanged, ui->widget, &CzujnikiAnalogoweOkno::updateValue);
    //connect(urzadzenia, &Urzadzenia::analogValueChanged, wykresy,  &WykresyOkno::updateValue);
    //connect(urzadzenia, &Urzadzenia::analogValueChanged, loger,    &Logger::updateValue);

    connect(urzadzenia, &Urzadzenia::digitalRead,   ui->frCzujniki, &OknoStatusowe::setDigitalValue);

    connect(urzadzenia, &Urzadzenia::usb6210,  ui->frCzujniki, &OknoStatusowe::setUSB6210);
    connect(urzadzenia, &Urzadzenia::usb6501,  ui->frCzujniki, &OknoStatusowe::setUSB6501);
    connect(urzadzenia, &Urzadzenia::dozownik, ui->frCzujniki, &OknoStatusowe::setDozownik);
    


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

    QAction * debugDozo = new QAction(this);
    debugDozo->setObjectName("debugactiondozo");
    debugDozo->setText(QString::fromUtf8("Dozownik - test"));
    ui->menuBar->addAction(debugDozo);
    connect(debugDozo, &QAction::triggered, this, &GlowneOkno::dozownikTest);


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

    testy[selectedTest] = new TestTabsWidget(projekty[selectedProject],
                                            settings,
                                             ui->testyStackedWidget);
    ui->testyStackedWidget->addWidget(testy[selectedTest]);
    ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);
    testy[selectedTest]->createTestWizard()->init(urzadzenia, settings, qtreewidgetitem->data(0, Qt::DisplayRole).toString());

    testy[selectedTest]->setActive();


    ui->treeWidget->setCurrentItem(selectedTest);
    mapTesty[selectedTest] = selectedProject;

    connect(urzadzenia, &Urzadzenia::digitalRead,        testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeDigitalIn);
    connect(urzadzenia, &Urzadzenia::analogValueChanged, testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeAnalog);

    changeSelectedTest();
    //resize(1024,768);
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
    static unsigned int nrTest = 1;
    if (selectedProject == nullptr)
        return;

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(selectedProject, QStringList(QString("Test %1").arg(nrTest++)));
    selectedProject->addChild(qtreewidgetitem);

    selectedTest = qtreewidgetitem;
    testy[selectedTest] = new TestTabsWidget(projekty[selectedProject],
                                            settings,
                                            ui->testyStackedWidget);
    ui->testyStackedWidget->addWidget(testy[selectedTest]);
    ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);

    testy[selectedTest]->createTestWizard()->init(urzadzenia, settings,
                                                  qtreewidgetitem->data(0, Qt::DisplayRole).toString());

    testy[selectedTest]->setActive();

    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeTestName, this, &GlowneOkno::changeTestName);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::finishedTest, this, &GlowneOkno::finishedTest);
    connect(urzadzenia, &Urzadzenia::digitalRead,        testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeDigitalIn);
    connect(urzadzenia, &Urzadzenia::analogValueChanged, testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeAnalog);


    ui->treeWidget->setCurrentItem(selectedTest);
    mapTesty[selectedTest] = selectedProject;
    changeSelectedTest();
    disableNowyTest(true);

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

void GlowneOkno::wybierzCzujke(int /*id*/)
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
    //dlgUrz->show();
}

void GlowneOkno::dozownikTest()
{
    DozownikSettings * dlg = new DozownikSettings(this);
    //dlg->setSmg(dlgUrz->getSerial());
    dlg->exec();
    delete dlg;
}

void GlowneOkno::valueChanged(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8)
{
    (void)val1;
    (void)val2;
    (void)val3;
    (void)val4;
    (void)val5;
    (void)val6;
    (void)val7;
    (void)val8;
    //emit analogValueChanged(id, mv);
}

void GlowneOkno::resizeEvent(QResizeEvent *event)
{
    ui->widget->setHorizontalSize(event->size().width());
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

void GlowneOkno::disableNowyTest(bool dis)
{
    ui->treeWidget->setEnabled(!dis);
    ui->actionDodaj_film->setEnabled(!dis);
    ui->actionDodaj_zdj_cie->setEnabled(!dis);
    ui->actionNowy_Test->setEnabled(!dis);
    ui->actionNowy_projekt->setEnabled(!dis);
}




void GlowneOkno::on_actionUstawienia_triggered()
{
    UstawieniaDozownika * dlg = new UstawieniaDozownika(settings, this);
    dlg->exec();
}

void GlowneOkno::changeTestName(const QString &name)
{
    if (selectedTest) {
        selectedTest->setData(0, Qt::DisplayRole, QVariant::fromValue(name));
    }
}

void GlowneOkno::finishedTest(bool success)
{
    qDebug("%s:%d %d",__FILE__,__LINE__,success);
    disableNowyTest(false);
    if (!success) {
        selectedProject->removeChild(selectedTest);
        delete testy[selectedTest];
        testy.remove(selectedTest);
        selectedTest = nullptr;
        changeSelectedTest();
    }
}

