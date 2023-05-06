#include "testtabswidget.h"
#include "ui_testtabswidget.h"
#include "pictureframe.h"

#include <QFileDialog>
#include <QDir>
#include <QCheckBox>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>

#include "ustawienia.h"
#include "pdfcreator.h"
/*
TestTabsWidget::TestTabsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget)
{
    ui->setupUi(this);
}
*/

#define SETCZUJANAL(A, N, M, X, P, V, W) do { Ustawienia::CzujnikAnalogowy temp = ust.getCzujnikAnalogowyUstawienia(A); \
    ui->check_AddPdf_##N->setText(temp.name); \
    ui->unit_1_##N->setText(temp.unit); \
    ui->unit_2_##N->setText(temp.unit); \
    czAnalRatio[A] = temp.convert; \
    czAnalUnit[A] = temp.unit; \
    ui->dbmin_##N->setMin(M); \
    ui->dbmin_##N->setMax(temp.convert*X); \
    ui->dbmin_##N->setPrec(P); \
    ui->dbmin_##N->setValue(temp.convert*V); \
    ui->dbmax_##N->setMin(M); \
    ui->dbmax_##N->setMax(temp.convert*X); \
    ui->dbmax_##N->setPrec(P); \
    ui->dbmax_##N->setValue(temp.convert*W); \
    } while(false);

#define CONTB(X) connect(ui->X, &QToolButton::clicked, this, &TestTabsWidget::X##_clicked)

TestTabsWidget::TestTabsWidget(const QString &testName, const Ustawienia & ust_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget),
    ust(ust_)
{
    ui->setupUi(this);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);

    ui->stackedWidget->setCurrentIndex(0);
    testDane.setNazwaTestu(testName);
    ui->stackedWidget->setTestData(&testDane);
    qInfo() << __FILE__ << ":" << __LINE__ << "test=" << testDane.getNazwaTestu();

    SETCZUJANAL(a_voc1, 1, 0, 100, 2, 0, 10);
    SETCZUJANAL(a_voc2, 2, 0, 100, 2, 0, 10);
    SETCZUJANAL(a_o2, 3, 0, 100, 2, 15, 30);
    SETCZUJANAL(a_co2, 4, 0, 100, 2, 0, 20);
    SETCZUJANAL(a_8, 5, 0, 100, 2, 0, 10);
    SETCZUJANAL(a_temp_komory, 6, 0, 2000, 1, 15, 150);
    SETCZUJANAL(a_cisn_komora, 7, 0, 200, 1, 85, 150);
    SETCZUJANAL(a_temp_parownik, 8, 0, 2000, 1, 15, 150);

    connect(this, &TestTabsWidget::processImageSignal, this, &TestTabsWidget::processImageSlot, Qt::QueuedConnection);
    CONTB(tbOpen);
    CONTB(tbbegin);
    CONTB(tbminus100);
    CONTB(tbminus50);
    CONTB(tbminus10);
    CONTB(tbminus5);
    CONTB(tbminus1);
    CONTB(tbplay);
    CONTB(tbplus1);
    CONTB(tbplus10);
    CONTB(tbplus5);
    CONTB(tbplus50);
    CONTB(tbplus100);
    CONTB(tbend);
    connect(ui->pbAddImage_video2, &QPushButton::clicked, this, &TestTabsWidget::pbAddImage_video2_clicked);


    bool en = false;
    ui->tbbegin->setEnabled(en);
    ui->tbend->setEnabled(en);
    ui->tbminus1->setEnabled(en);
    ui->tbminus5->setEnabled(en);
    ui->tbminus10->setEnabled(en);
    ui->tbminus50->setEnabled(en);
    ui->tbminus100->setEnabled(en);
    ui->tbplay->setEnabled(en);
    ui->tbplus1->setEnabled(en);
    ui->tbplus5->setEnabled(en);
    ui->tbplus10->setEnabled(en);
    ui->tbplus50->setEnabled(en);
    ui->tbplus100->setEnabled(en);
    ui->pbAddImage_video2->setEnabled(en);

    playVideo = false;
}

TestTabsWidget::~TestTabsWidget()
{
    delete ui;
}

void TestTabsWidget::setActive()
{
    ui->tabWidget->setCurrentIndex(0);
}

CreateTestWizard * TestTabsWidget::createTestWizard() const
{
    return ui->stackedWidget;
}

QString TestTabsWidget::getTestName() const
{
    return testDane.getNazwaTestu();
}

void TestTabsWidget::finishedTest(const ProjectItem & projekt)
{
    qInfo() << __FILE__ << ":" << __LINE__ << "test=" << testDane.getNazwaTestu();
    testDane.setDateTime(projekt.getCreateDt());
    testDane.setMembers(projekt.getMembers().split("\n"));
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->tabWidget->setTabEnabled(3, true);
    //ui->tabFoto->setC
    QString workDir = projekt.getWorkDir();

    QString testname = testDane.getNazwaTestu();

    testWorkDir = QDir(workDir);
    if (!testWorkDir.exists()) {
        QMessageBox::information(this, tr("Stanowisko do badań eksplozji"), tr("Katalog projektu nie istnieje, Kliknij OK i wybierz katalog projektu"));
        workDir = QFileDialog::getExistingDirectory(this, tr("Wybierz katalog do zapisu danych z testu"));
        if (workDir.isEmpty())
            workDir = QDir::homePath();
    }
    bool cdircreate = testWorkDir.mkdir(testname);
    if (cdircreate) {
        testWorkDir.cd(testname);
        testWorkDirName = testWorkDir.absolutePath();
    } else {
        int try3 = 3;
        cdircreate = false;
        while (try3-- && !cdircreate) {
            QString ndir = QString("%1-%2").arg(testname).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
            cdircreate = testWorkDir.mkdir(ndir);
            if (cdircreate) {
                testWorkDir.cd(ndir);
                testWorkDirName = testWorkDir.absolutePath();
            }
        }
        if (!cdircreate) {
            QMessageBox::warning(this, tr("Stanowisko do badań eksplozji"), tr("Aplikacja nie może utworzyć katalogu dla danych testu"));
        }
    } //cdircreate
}

void TestTabsWidget::initfinishedTest()
{
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->tabWidget->setTabEnabled(3, true);
}

void TestTabsWidget::on_pbAddImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    //this->setCursor(Qt::WaitCursor);
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    if (fileNames.size() == 0)
        return;


    emit processImageSignal(fileNames);
}

void TestTabsWidget::processImageSlot(QStringList fileNames)
{
    //this->setCursor(Qt::WaitCursor);
    ui->frame_images->addImages(fileNames, testWorkDirName);
    //this->setCursor(Qt::ArrowCursor);
}

void TestTabsWidget::processVideSlot(QStringList filenames, QString dirdest)
{

}

#define SHOW_WYKRES(A, N, O1, O2)     pdf.setWykresVisible(A, ui->check_AddPdf_##N->isChecked(), \
    ui->dbmin_##N->value(), ui->dbmax_##N->value(), czAnalRatio[A], ui->page_##N->value(), O1, O2, czAnalUnit[A])

void TestTabsWidget::on_pbCreateRaport_clicked()
{
    PdfCreator pdf(testDane, ust);
    pdf.clearImage();
    pdf.setComment(ui->pdfComment->toHtml());

    SHOW_WYKRES(a_voc1, 1, tr("Wykres wartości stężenia czujnika VOC1:"), tr("Evikon E2638, etanol %LEL"));
    SHOW_WYKRES(a_voc2, 2, tr("Wykres wartości stężenia czujnika VOC2:"), tr("Evikon E2638, aceton %LEL"));
    SHOW_WYKRES(a_o2, 3, tr("Wykres wartości stężenia czujnika O2:"), tr("Evikon E2638, tlen 0-25%"));
    SHOW_WYKRES(a_co2, 4, tr("Wykres wartości stężenia czujnika CO2:"), tr("Vaisala GMP251, 0-20%"));
    SHOW_WYKRES(a_8, 5, tr("Wykres wartości stężenia czujnika wirtualnego:"), tr("Przeliczenia wskazań z VOC1, wg stałej wprowadzonej przez użytkownika"));
    SHOW_WYKRES(a_cisn_komora, 7, tr("Wykres wartości ciśnienia wewnątrz komory:"), tr("WIKA A-10, 0-2,5 bar abs"));
    SHOW_WYKRES(a_temp_parownik, 8, tr("Wykres wartości temperatury parownika:"), tr("Shimaden SR91, czujnik Pt100"));
    SHOW_WYKRES(a_temp_komory, 6, tr("Wykres wartości temperatury wewnątrz komory:"), tr("Shimaden SD17, termopara typ K, fi3mm"));

    foreach (auto im, ui->frame_images->getImagesDecription()) {
        pdf.addImage(im.first, im.second);
        qDebug() << im.first;
    }

    QPrinter printer(QPrinter::PrinterResolution);
    QTextDocument * textDocument = new QTextDocument;

    QString pdfBody = pdf.getBody();
    //qInfo() << pdfBody;
    textDocument->setHtml(pdfBody);
    printer.setOutputFormat(QPrinter::PdfFormat);
    QString baseName = QString("Raport_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss"));
    QString fileName = testWorkDir.absoluteFilePath(baseName);

    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setFullPage(true);
    textDocument->print(&printer);
    QMessageBox::information(this, tr("Stanowisko do analizy wybuchów"), QString(tr("Wygenerowano raport do pliku %1")).arg(fileName));
}

QDataStream & operator<<(QDataStream & ds, const TestTabsWidget & item)
{

    TestData testDane;

    qInfo() << "TestTabsWidget::Test";
    ds << item.testDane;

    qInfo() << "TestTabsWidget::testWorkDir " << item.testWorkDir.absolutePath();
    qInfo() << "TestTabsWidget::testWorkDirName " << item.testWorkDirName;
    qInfo() << "images=[";
    foreach (auto im, item.images) {
        qInfo() << im << ",";
    }
    qInfo() << "]";
    ds << item.testWorkDir.absolutePath() << item.images;
    qInfo() << "TestTabsWidget::Done";
    return ds;
}

QDataStream & operator>>(QDataStream & ds, TestTabsWidget & item)
{
    QString sTestWorkDir;
    QStringList files;
    qInfo() << "TestTabsWidget:Start";
    ds >> item.testDane >> sTestWorkDir >> item.images;

    item.testWorkDir = QDir(sTestWorkDir);
    item.testWorkDirName = sTestWorkDir;
    
    qInfo() << "TestTabsWidget::testWorkDir " << item.testWorkDir.absolutePath();
    qInfo() << "TestTabsWidget::testWorkDirName " << item.testWorkDirName;
    qInfo() << "images=[";
    foreach (auto im, item.images) {
        qInfo() << im << ",";
    }
    qInfo() << "]";

    item.initfinishedTest();
    qInfo() << "TestTabsWidget:End";
    
    return ds;
}


void TestTabsWidget::tbOpen_clicked()
{
    QString fileDir = QFileDialog::getExistingDirectory(this, tr("Wybierz katalog z filmem"));
    if (fileDir.isEmpty())
        return;
    QDir directory(fileDir);
    QDir targetDir(testWorkDir);
    targetDir.mkdir("movie");
    targetDir.cd("movie");
    QStringList images = directory.entryList(QStringList() << "*.tiff" << "*.TIFF",QDir::Files, QDir::Name);
    QStringList sourceFiles, destFiles;
    foreach(QString filename, images) {
        QString fsrc = directory.absoluteFilePath(filename);
        QString fdst = targetDir.absoluteFilePath(filename);
        destFiles << fdst;
        sourceFiles << fsrc;
    }

    bool en = true;
    ui->tbbegin->setEnabled(en);
    ui->tbend->setEnabled(en);
    ui->tbminus1->setEnabled(en);
    ui->tbminus5->setEnabled(en);
    ui->tbminus10->setEnabled(en);
    ui->tbminus50->setEnabled(en);
    ui->tbminus100->setEnabled(en);
    ui->tbplay->setEnabled(en);
    ui->tbplus1->setEnabled(en);
    ui->tbplus5->setEnabled(en);
    ui->tbplus10->setEnabled(en);
    ui->tbplus50->setEnabled(en);
    ui->tbplus100->setEnabled(en);
    ui->pbAddImage_video2->setEnabled(en);

    ui->videoWidget->setFrameNr(ui->frameString);
    ui->videoWidget->setFiles(sourceFiles);
}


void TestTabsWidget::tbbegin_clicked()
{
    ui->videoWidget->first();
}


void TestTabsWidget::tbminus100_clicked()
{
    ui->videoWidget->down(100);
}


void TestTabsWidget::tbminus50_clicked()
{
    ui->videoWidget->down(50);
}


void TestTabsWidget::tbminus10_clicked()
{
    ui->videoWidget->down(10);
}


void TestTabsWidget::tbminus5_clicked()
{
    ui->videoWidget->down(5);
}


void TestTabsWidget::tbminus1_clicked()
{
    ui->videoWidget->down(1);
}


void TestTabsWidget::tbplay_clicked()
{
    playVideo = !playVideo;
    if (playVideo) {
        ui->tbplay->setText("Pause");
    } else {
        ui->tbplay->setText("Play");
    }
    ui->videoWidget->play(playVideo);
}


void TestTabsWidget::tbplus1_clicked()
{
    ui->videoWidget->up(1);
}

void TestTabsWidget::tbplus5_clicked()
{
    ui->videoWidget->up(5);
}

void TestTabsWidget::tbplus10_clicked()
{
    ui->videoWidget->up(10);
}


void TestTabsWidget::tbplus50_clicked()
{
    ui->videoWidget->up(50);
}


void TestTabsWidget::tbplus100_clicked()
{
    ui->videoWidget->up(100);
}


void TestTabsWidget::tbend_clicked()
{
    ui->videoWidget->last();
}

void TestTabsWidget::pbAddImage_video2_clicked()
{
    QStringList file;
    file << ui->videoWidget->getCurrentImage();
    emit processImageSignal(file);
}

