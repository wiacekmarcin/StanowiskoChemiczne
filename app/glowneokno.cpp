#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "czujnikianalogoweustawieniaframe.h"

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
#include "projectitem.h"
#include "nowyprojectdlg.h"

#include "testtabswidget.h"
#include "urzadzenia.h"
#include "dozowniksettings.h"
#include "ustawieniadozownika.h"
#include "ustawienia.h"
#include "ustawieniadialog.h"

#include "wersjadlg.h"

GlowneOkno::GlowneOkno(Ustawienia & ust, Urzadzenia * urzadz, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno),
    urzadzenia(urzadz),
    settings(ust),
    selectedProject(nullptr),
    selectedTest(nullptr),
    thReadDigAnal(this),
    thDozownik(this),
    thWykresy(this),
    thTest(this)
{
    thReadDigAnal.setObjectName("DigitalReadThr");
    thDozownik.setObjectName("DozownikThr");
    thWykresy.setObjectName("Wykresy");
    thTest.setObjectName("Test");

    ui->setupUi(this);
    ui->frCzujniki->setLabels(settings);
    ui->analog->setParams(settings);
    ui->wyjscia->setLabels(settings);
    
    urzadz->setThreads(&thReadDigAnal, &thDozownik);
    
    thReadDigAnal.start();
    thDozownik.start();
    
    //thWykresy.start();
    //thTest.start();

    connect(urzadzenia, &Urzadzenia::analogValueChanged, ui->analog, &CzujnikiAnalogoweOkno::updateValue, Qt::DirectConnection);
    //connect(urzadzenia, &Urzadzenia::analogValueChanged, wykresy,  &WykresyOkno::updateValue, Qt::DirectConnection);
    //connect(urzadzenia, &Urzadzenia::analogValueChanged, loger,    &Logger::updateValue, Qt::DirectConnection);

    connect(urzadzenia, &Urzadzenia::digitalReadValueChanged,  ui->frCzujniki, &OknoStatusowe::setDigitalValue, Qt::DirectConnection);
    connect(urzadzenia, &Urzadzenia::digitalWriteValueChanged, ui->wyjscia,    &OknoStanoweWyjscia::setOnOff,   Qt::QueuedConnection);

    connect(ui->wyjscia, &OknoStanoweWyjscia::writeValue, urzadzenia, &Urzadzenia::digitalWrite, Qt::DirectConnection);

    connect(urzadzenia, &Urzadzenia::usb6210,  ui->frCzujniki, &OknoStatusowe::setUSB6210, Qt::QueuedConnection);
    connect(urzadzenia, &Urzadzenia::usb6501,  ui->frCzujniki, &OknoStatusowe::setUSB6501, Qt::QueuedConnection);
    connect(urzadzenia, &Urzadzenia::dozownik, ui->frCzujniki, &OknoStatusowe::setDozownik,Qt::QueuedConnection);
    

    urzadzenia->readInputs();
    signalMapper = new QSignalMapper(this);

    for (int i = 0; i < settings.maxCzujekAnal; ++i) {
        act_wykresy[i] = new QAction(this);
        act_wykresy[i]->setObjectName(QString("actionCzujnikChart_%1").arg(i));
        act_wykresy[i]->setCheckable(true);
        act_wykresy[i]->setText(settings.getCzAnalName((analogIn)i));
        connect(act_wykresy[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(act_wykresy[i],     i);
        //connect(act_wykresy[i], &QAction::triggered, [this, i] { wybierzCzujke(i); });

        act_wyzwal[i] = new QAction(this);
        act_wyzwal[i]->setObjectName(QString("actionCzujnikTrigger_%1").arg(i));
        act_wyzwal[i]->setCheckable(true);
        act_wyzwal[i]->setText(settings.getCzAnalName((analogIn)i));
        connect(act_wyzwal[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(act_wyzwal[i],     settings.maxCzujekAnal + i);
        //connect(act_wyzwal[i], &QAction::triggered, [this, i] { wybierzCzujke(settings.maxCzujek+i); });

        ui->menuPodgl_d_wej->addAction(act_wykresy[i]);
        ui->menuWyzwalanie_wyj->addAction(act_wyzwal[i]);
    }

    connect (signalMapper, SIGNAL( mapped(int) ), this, SLOT(wybierzCzujke(int))) ;


    changeSelectedTest();
// testowy test
/*
    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("Testowy projekt")));
    projekty[qtreewidgetitem] = ProjectItem("Testowy projekt", "Członek 1\nCzłonek 2\nCzłonek 3",
                                            "/home/test", "Komentarz", "Dzisiejsza data", QDateTime::currentDateTime());
    selectedProject = qtreewidgetitem;
    ui->treeWidget->setCurrentItem(qtreewidgetitem);
*/


    urzadz->digitalWriteAll(0x2);
}

GlowneOkno::~GlowneOkno()
{
    finishedTest(false);
    urzadzenia->setStop();
    thReadDigAnal.quit();
    thDozownik.quit();
    thReadDigAnal.wait();
    thDozownik.wait();
    //thReadDigAnal.terminate();
    //thDozownik.terminate();

    delete ui;
}

void GlowneOkno::on_actionNowy_projekt_triggered()
{
    auto * dlg = new NowyProjectDlg(this);
    if (dlg->exec() == QDialog::Rejected) {
        return;
    }

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(ui->treeWidget, QStringList(dlg->getName()));
    projekty[qtreewidgetitem] = ProjectItem(dlg->getName(), dlg->getMembers(), dlg->getWorkDir(), dlg->getComment(),
                                            dlg->getDate(), dlg->getDateTime());

    mapProjektTesty[qtreewidgetitem] = QList<QTreeWidgetItem*>();

    selectedProject = qtreewidgetitem;
    ui->treeWidget->setCurrentItem(qtreewidgetitem);
    changeSelectedTest();

}

void GlowneOkno::on_actionNowy_Test_triggered()
{
    static unsigned int nrTest = 1;
    if (selectedProject == nullptr)
        return;

    QString testName = QString("Test %1").arg(nrTest++);
    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(selectedProject, QStringList(testName));
    selectedProject->addChild(qtreewidgetitem);
    selectedTest = qtreewidgetitem;

    mapProjektTesty[selectedProject].append(qtreewidgetitem);
    
    testy[selectedTest] = new TestTabsWidget(projekty[selectedProject], testName,
                                            ui->testyStackedWidget);

    ui->testyStackedWidget->addWidget(testy[selectedTest]);
    ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);
    testy[selectedTest]->createTestWizard()->init(settings,
                                                  qtreewidgetitem->data(0, Qt::DisplayRole).toString());

    ui->treeWidget->setCurrentItem(selectedTest);
    mapTesty[selectedTest] = selectedProject;
    testy[selectedTest]->setActive();


    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeTestName,
            this, &GlowneOkno::changeTestName, Qt::QueuedConnection);
    connect(testy[selectedTest]->createTestWizard(),
            &CreateTestWizard::finishedTest, this, &GlowneOkno::finishedTest, Qt::QueuedConnection);
    connect(testy[selectedTest]->createTestWizard(),
            &CreateTestWizard::finishedTest, testy[selectedTest], &TestTabsWidget::finishedTest, Qt::QueuedConnection);

    connect(urzadzenia, &Urzadzenia::digitalReadValueChanged,
            testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeDigitalIn,
            Qt::DirectConnection);
    connect(urzadzenia, &Urzadzenia::analogValueChanged,
            testy[selectedTest]->createTestWizard(), &CreateTestWizard::changeAnalog,
            Qt::DirectConnection);
    connect(urzadzenia, &Urzadzenia::setCykleDone,
            testy[selectedTest]->createTestWizard(), &CreateTestWizard::dozownikDone,
            Qt::QueuedConnection);
    connect(urzadzenia, &Urzadzenia::setStepsDone,
            testy[selectedTest]->createTestWizard(), &CreateTestWizard::dozownikDone,
            Qt::QueuedConnection);
    connect(urzadzenia, &Urzadzenia::checkPositionHomeDone,
            testy[selectedTest]->createTestWizard(), &CreateTestWizard::checkPositionHomeDone,
            Qt::QueuedConnection);

    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::setDigitalOut,
            urzadzenia, &Urzadzenia::digitalWrite, Qt::DirectConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::cykleDozownik,
            urzadzenia, &Urzadzenia::setCykle, Qt::DirectConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::dozownikMl,
            urzadzenia, &Urzadzenia::setMl, Qt::DirectConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::writeOutValues,
            urzadzenia, &Urzadzenia::digitalWrite, Qt::DirectConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::zaplon,
            urzadzenia, &Urzadzenia::zaplon, Qt::DirectConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::checkPositionHome,
            urzadzenia, &Urzadzenia::checkPositionHome, Qt::DirectConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::readsInputs,
            urzadzenia, &Urzadzenia::readInputs, Qt::QueuedConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::resetDozownik,
            urzadzenia, &Urzadzenia::resetDozownik, Qt::QueuedConnection);


    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::debug,
            urzadzenia, &Urzadzenia::gui_debug, Qt::QueuedConnection);
    connect(testy[selectedTest]->createTestWizard(), &CreateTestWizard::error,
            urzadzenia, &Urzadzenia::gui_error, Qt::QueuedConnection);

    connect(testy[selectedTest]->createTestWizard()->getTestData(), &TestData::debug,
            urzadzenia, &Urzadzenia::test_debug, Qt::QueuedConnection);
    connect(testy[selectedTest]->createTestWizard()->getTestData(), &TestData::error,
            urzadzenia, &Urzadzenia::test_error, Qt::QueuedConnection);

    changeSelectedTest();
    disableNowyTest(true);
    urzadzenia->readInputs();
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
    //
}

void GlowneOkno::dozownikTest()
{
    DozownikSettings * dlg = new DozownikSettings(this);
    //dlg->setSmg(dlgUrz->getSerial());
    dlg->exec();
    delete dlg;
}

void GlowneOkno::resizeEvent(QResizeEvent *event)
{
    ui->analog->setHorizontalSize(event->size().width());
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
        act_wykresy[i]->setText(settings.getCzAnalName((analogIn)i));
        act_wyzwal[i]->setText(settings.getCzAnalName((analogIn)i));
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
    disableNowyTest(false);
    if (!success) {
        selectedProject->removeChild(selectedTest);
        delete testy[selectedTest];
        testy.remove(selectedTest);
        selectedTest = nullptr;
        changeSelectedTest();
    }
}


void GlowneOkno::on_actionWersja_triggered()
{
    WersjaDlg * dlg = new WersjaDlg(this);
    dlg->exec();
    delete dlg;
}


void GlowneOkno::on_actionUstawieniaOgolne_triggered()
{
    UserPrivilige user = U_STUDENT;
    UstawieniaDialog *dlg = new UstawieniaDialog(user, settings, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        //dlg->saveData(settings);
        ui->analog->setParams(settings);
        setActionText();
    }
}


void GlowneOkno::on_actionZapisz_triggered()
{
    QFile file("stanowisko.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << (quint32)0xA0B0C0D0;
    out << (qint32)104;

    out.setVersion(QDataStream::Qt_5_15);

    out << (quint32)mapProjektTesty.size();
    qInfo() << (quint32)mapProjektTesty.size();

    for (QHash<QTreeWidgetItem*, QList<QTreeWidgetItem*>>::iterator it = mapProjektTesty.begin();
         it != mapProjektTesty.end(); ++it) {
        qInfo() << "Projekt";
        out << projekty[it.key()];
        out << (quint32)it.value().size();
        for (QList<QTreeWidgetItem*>::iterator itt = it.value().begin();
             itt != it.value().end(); ++itt) {
            qInfo() << "nazwa" << testy[*itt]->getTestName();
            out << testy[*itt]->getTestName();
            out << *testy[*itt];
        }
        out << (quint32)0xA0B0C0D0;
    }
}


void GlowneOkno::on_actionOtw_rz_triggered()
{
    QFile file("stanowisko.dat");
    file.open(QIODevice::ReadOnly);

    QDataStream in(&file);

    // Read and check the header
    quint32 magic;
    in >> magic;
    if (magic != 0xA0B0C0D0)
        return ; //ERROR

    // Read the version
    qint32 version;
    in >> version;

    if (version != 104)
        return ; //inna wersja

    in.setVersion(QDataStream::Qt_5_15);

    quint32 projektysize;
    quint32 testysize;
    in >> projektysize;
    for(uint i = 0 ; i < projektysize; ++i) {
        ProjectItem pr;
        in >> pr;
        QTreeWidgetItem *projectItem = new QTreeWidgetItem(ui->treeWidget, QStringList() << pr.getName());
        projekty[projectItem] = pr;

        mapProjektTesty[projectItem] = QList<QTreeWidgetItem*>();
        in >> testysize;
        for(uint j = 0 ; j < testysize; ++j) {
            QString testname;
            in >> testname;
            QTreeWidgetItem *testIitem = new QTreeWidgetItem(projectItem, QStringList(testname));
            projectItem->addChild(testIitem);
            mapProjektTesty[projectItem].append(testIitem);

            TestTabsWidget *tb = new TestTabsWidget(pr, testname, ui->testyStackedWidget);
            in >> (*tb);

            testy[testIitem] = tb;
            mapTesty[testIitem] = projectItem;
            ui->testyStackedWidget->addWidget(tb);
            tb->createTestWizard()->init(settings, testname);
            tb->createTestWizard()->finishedTest(true);
            tb->finishedTest(true);

        }
        in >> magic;
        if (magic != 0xA0B0C0D0)
            continue;
    }
}

