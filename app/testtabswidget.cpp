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
    SETCZUJANAL(a_temp_komory, 6, 0, 2000, 1, 15, 600);
    SETCZUJANAL(a_cisn_komora, 7, 0, 200, 1, 10, 150);
    SETCZUJANAL(a_temp_parownik, 8, 0, 2000, 1, 15, 600);

    connect(this, &TestTabsWidget::processImageSignal, this, &TestTabsWidget::processImageSlot, Qt::QueuedConnection);
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
    //workDir = "E:\\devel\\workdir";
    QString testname = testDane.getNazwaTestu();
    //testname= "Test";
    testWorkDir = QDir(workDir);
    if (!testWorkDir.exists())
        return;
    bool cdircreate = testWorkDir.mkdir(testname);
    if (cdircreate) {
        testWorkDir.cd(testname);
        testWorkDirName = testWorkDir.absolutePath();
    } else {

        int num = 1;
        qInfo() << __FILE__ << ":" << __LINE__ << workDir;
        while (!cdircreate) {
            QString ndir = QString("%1-%2").arg(testname).arg(num++);
            qInfo() << ndir;
            cdircreate = testWorkDir.mkdir(ndir);
            if (cdircreate) {
                testWorkDir.cd(ndir);
                testWorkDirName = testWorkDir.absolutePath();
            }
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
    //dialog.setDirectory("E:\\devel\\StanowiskoChemiczne-source\\StanowiskoChemiczne\\img");
    QStringList fileNames;
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    //this->setCursor(Qt::WaitCursor);
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    if (fileNames.size() == 0)
        return;

    qDebug() << fileNames;
    emit processImageSignal(fileNames);
}

void TestTabsWidget::processImageSlot(QStringList fileNames)
{
    //this->setCursor(Qt::WaitCursor);
    ui->frame_images->addImages(fileNames, testWorkDirName);
    //this->setCursor(Qt::ArrowCursor);
}

#define SHOW_WYKRES(A, N, O1, O2)     pdf.setWykresVisible(A, ui->check_AddPdf_##N->isChecked(), \
    ui->dbmin_##N->value(), ui->dbmax_##N->value(), czAnalRatio[A], ui->page_##N->value(), O1, O2, czAnalUnit[A])

void TestTabsWidget::on_pbCreateRaport_clicked()
{
    PdfCreator pdf(testDane, ust);
    pdf.clearImage();
    pdf.setComment(ui->pdfComment->toHtml());

    SHOW_WYKRES(a_voc1, 1, "Wykres wartości stężenia czujnika VOC1:", "Evikon E2638, etanol %LEL");
    SHOW_WYKRES(a_voc2, 2, "Wykres wartości stężenia czujnika VOC2:", "Evikon E2638, etanol %LEL");
    SHOW_WYKRES(a_o2, 3, "Wykres wartości stężenia czujnika O2:", "Evikon E2638, tlen 0-25%");
    SHOW_WYKRES(a_co2, 4, "Wykres wartości stężenia czujnika CO2:", "Vaisala GMP251, 0-20%");
    SHOW_WYKRES(a_8, 5, "Wykres wartości stężenia czujnika wirtualnego:", "Przeliczenia wskazań z VOC1, wg stałej wprowadzonej przez użytkownika");
    SHOW_WYKRES(a_cisn_komora, 7, "Wykres wartości ciśnienia wewnątrz komory:", "WIKA A-10, 0-2,5 bar abs");
    SHOW_WYKRES(a_temp_parownik, 8, "Wykres wartości temperatury parownika:", "Shimaden SR91, czujnik Pt100");
    SHOW_WYKRES(a_temp_komory, 6, "Wykres wartości temperatury wewnątrz komory:", "Shimaden SD17, termopara typ K, fi3mm");

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
    qDebug() << "Nazwa raportu" << fileName;
    printer.setOutputFileName(baseName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setFullPage(true);
    textDocument->print(&printer);

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

