#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "czujnikianalogoweustawieniaframe.h"
#include "logowaniedlg.h"


//#include <QMediaPlayer>
//#include <QMediaPlaylist>
#include <QFile>
//#include <QVideoWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QResizeEvent>
#include <QWidgetAction>
#include <QLabel>
#include <QHBoxLayout>

#include "videowidget.h"
#include "createtestwizard.h"
#include "projectitem.h"
#include "nowyprojectdlg.h"

#include "testtabswidget.h"
#include "urzadzenia.h"

#include "ustawieniadozownika.h"
#include "ustawienia.h"
#include "ustawieniadialog.h"
#include "ustawieniatestu.h"

#include "wersjadlg.h"

GlowneOkno::GlowneOkno(UserPrivilige user_, Ustawienia & ust, Urzadzenia * urzadz, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno),
    user(user_),
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


    userLogInfo = new QLabel(ui->menuBar);
    userLogInfo->setText(QString("Zalogowany jako : %1").arg((user == U_STUDENT ? "Student" : (user == U_ADMIN ? "Administrator" : "Serwisant"))));
    userLogInfo->setContentsMargins(15, 6, 15, 6);

    QWidgetAction *widgetAction=new QWidgetAction(this);
    widgetAction->setObjectName("Zalogowanyjako");
    widgetAction->setDefaultWidget(userLogInfo);

    //ui->menuBar->addAction(widgetAction);
    ui->menuLogowanie->addAction(widgetAction);
    ui->menuLogowanie->addSeparator();
    ui->menuLogowanie->addAction(QString("Logowanie"), this, &GlowneOkno::onLogowanieTriggered);
    ui->menuLogowanie->addAction(QString("Wylogowanie"), this, &GlowneOkno::onWylogowanieTriggered);

    for (int i = 0; i < settings.maxCzujekAnal; ++i) {
        act_wykresy[i] = new QAction(this);
        act_wykresy[i]->setObjectName(QString("actionCzujnikChart_%1").arg(i));
        act_wykresy[i]->setCheckable(true);
        act_wykresy[i]->setText(settings.getCzAnalName((analogIn)i));
        connect(act_wykresy[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(act_wykresy[i],     i);
        //connect(act_wykresy[i], &QAction::triggered, [this, i] { wybierzCzujke(i); });
        ui->menuPodgl_wyj->addAction(act_wykresy[i]);
    }

    connect (signalMapper, SIGNAL( mapped(int) ), this, SLOT(wybierzCzujke(int))) ;


    changeSelectedTest();
// testowy test

    QTreeWidgetItem *qtreewidgetitem = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("Testowy projekt")));
    projekty[qtreewidgetitem] = ProjectItem("Testowy projekt", "Członek 1\nCzłonek 2\nCzłonek 3",
                                            "/home/test", "Komentarz", "Dzisiejsza data", QDateTime::currentDateTime());
    selectedProject = qtreewidgetitem;
    ui->treeWidget->setCurrentItem(qtreewidgetitem);

    on_actionNowy_Test_triggered();

    urzadz->digitalWriteAll(0x2);
}

GlowneOkno::~GlowneOkno()
{
    //thReadDigAnal.deleteLater();
    //thDozownik.deleteLater();
    qDebug() << "APP: thReadDigAnal.isRunning()=" << thReadDigAnal.isRunning();
    qDebug() << "APP: thDozownik.isRunning()= " << thDozownik.isRunning();
    urzadzenia->setStop();
    thReadDigAnal.quit();
    thDozownik.quit();

    thReadDigAnal.wait();
    thDozownik.wait();
    qDebug() << "APP: thReadDigAnal.isRunning()=" << thReadDigAnal.isRunning();
    qDebug() << "APP: thDozownik.isRunning()= " << thDozownik.isRunning();

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
    
    testy[selectedTest] = new TestTabsWidget(projekty[selectedProject], testName, settings,
                                            ui->testyStackedWidget);

    ui->testyStackedWidget->addWidget(testy[selectedTest]);
    ui->testyStackedWidget->setCurrentWidget(testy[selectedTest]);
    testy[selectedTest]->createTestWizard()->init(&settings, user,
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

void GlowneOkno::resizeEvent(QResizeEvent *event)
{
    ui->analog->setHorizontalSize(event->size().width());
}

void GlowneOkno::changeSelectedTest()
{
    ui->actionNowy_Test->setDisabled(selectedProject == nullptr);
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
    ui->actionNowy_Test->setEnabled(!dis);
    ui->actionNowy_projekt->setEnabled(!dis);
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

void GlowneOkno::on_actionZapisz_triggered()
{
    QFile file("stanowisko.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << (quint32)0xA0B0C0D0;
    out << (qint32)104;

    //out.setVersion(QDataStream::Qt_5_15);
    out.setVersion(QDataStream::Qt_5_12);

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

    //in.setVersion(QDataStream::Qt_5_15);
    in.setVersion(QDataStream::Qt_5_12);

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

            TestTabsWidget *tb = new TestTabsWidget(pr, testname, settings, ui->testyStackedWidget);
            in >> (*tb);

            testy[testIitem] = tb;
            mapTesty[testIitem] = projectItem;
            ui->testyStackedWidget->addWidget(tb);
            tb->createTestWizard()->initFromFile();
            //tb->createTestWizard()->finishedTest(true);
            tb->finishedTest(true);

        }
        in >> magic;
        if (magic != 0xA0B0C0D0)
            continue;
    }
}

void GlowneOkno::onLogowanieTriggered()
{
    LogowanieDlg * dlg = new LogowanieDlg();
    if (dlg->exec() == QDialog::Rejected) {
        return;
    }
    user = dlg->getUser();
    userLogInfo->setText(QString("Zalogowany jako : %1").arg((user == U_STUDENT ? "Student" : (user == U_ADMIN ? "Administrator" : "Serwisant"))));
    delete dlg;
}

void GlowneOkno::onWylogowanieTriggered()
{
    user = U_STUDENT;
    userLogInfo->setText(QString("Zalogowany jako : %1").arg((user == U_STUDENT ? "Student" : (user == U_ADMIN ? "Administrator" : "Serwisant"))));
}

void GlowneOkno::onUstawieniaTriggered()
{


    UstawieniaDialog *dlg2 = new UstawieniaDialog(user, settings, this);
    if (dlg2->exec() == QDialog::Accepted)
    {
        //dlg->saveData(settings);
        ui->analog->setParams(settings);
        setActionText();
    }
}



void GlowneOkno::on_actionSygna_y_analogowe_triggered()
{

}


void GlowneOkno::on_actionDozowniki_triggered()
{
    UstawieniaDozownika * dlg = new UstawieniaDozownika(settings, user, this);
    dlg->exec();
    delete dlg;
}


void GlowneOkno::on_actionUstawienia_testu_triggered()
{
    UstawieniaTestu * dlg = new UstawieniaTestu(settings, user, this);
    dlg->exec();
    delete dlg;
}

