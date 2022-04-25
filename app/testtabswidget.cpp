#include "testtabswidget.h"
#include "ui_testtabswidget.h"
#include "pictureframe.h"

#include <QFileDialog>
#include <QDir>
#include <QCheckBox>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>


#include "ustawienia.h"

TestTabsWidget::TestTabsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget)
{
    ui->setupUi(this);
}

TestTabsWidget::TestTabsWidget(const ProjectItem &pr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget),
    projekt(pr)
{
    ui->setupUi(this);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);

    ui->stackedWidget->setCurrentIndex(0);
    testDane.setDateTime(pr.getCreateDt());
    testDane.setMembers(pr.getMembers().split("\n"));
    ui->stackedWidget->setTestData(&testDane);

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

void TestTabsWidget::finishedTest(bool success)
{
    qInfo() << __FILE__ << ":" << __LINE__;
    if (success) {
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
        ui->tabWidget->setTabEnabled(3, true);
        //ui->tabFoto->setC
        QString workDir = projekt.getWorkDir();
        workDir = "E:\\devel\\workdir";
        QString testname = testDane.getNazwaTestu();
        testname= "Test";
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
    } //success
}

void TestTabsWidget::on_pbAddImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory("E:\\devel\\StanowiskoChemiczne-source\\StanowiskoChemiczne\\img");
    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    if (fileNames.size() == 0)
        return;
    qDebug() << testWorkDirName;
    foreach(auto fName, fileNames) {
        QFileInfo f(fName);
        QString fn = f.fileName();
        QString basename = f.baseName();
        QString fdst = testWorkDir.absoluteFilePath(fn);
        QFile fp(fName);
        if (!fp.copy(fdst)) {
            qDebug() << "nie udalo sie skopiowac: " << fName << " -> " << fdst;
            continue;
        }

        qDebug() << "fName" << fName << fdst;
        QImage im(fdst);
        if (im.isNull()) {
            qDebug() << "nie udalo sie otworzyć: " << fdst;
            continue;
        }

        //QSize is = im.size();
        QImage c = im.scaledToWidth(800);
        //QImage png = c.con

        QString fileNamePng = testWorkDir.absoluteFilePath(basename) + QString(".png");
        qDebug() <<  "save: " << fileNamePng;
        if(!c.save(fileNamePng, "PNG")) {
            qDebug() << "canot save" ;
            continue;
        }
        ui->gridLayoutImages->addWidget(new PictureFrame(this, fileNamePng),
                                        images.size() / 3, images.size() % 3);

        QCheckBox * checkBox = new QCheckBox(ui->scrollAreaWidgetContents_2);
        checkBox->setObjectName(QString::fromUtf8("checkBox")+QString::number(images.size()));
        checkBox->setText(basename);
        ui->gridLayoutCheckbox->addWidget(checkBox, images.size() / 3, images.size() % 3);
        m_imageCheckBox << checkBox;
        images << fileNamePng;

    }

}

#define SHOW_WYKRES(A, N, O, U)     testDane.setWykresVisible(A, ui->check_AddPdf_##N->isChecked(), ui->dbmin_##N->value(), ui->dbmax_##N->value(), O, U)

void TestTabsWidget::on_pbCreateRaport_clicked()
{
    testDane.clearImage();
    SHOW_WYKRES(a_vol1, 1, "voc1", "%");
    SHOW_WYKRES(a_vol2, 2, "voc2", "%");
    SHOW_WYKRES(a_o2, 3, "o2", "%");
    SHOW_WYKRES(a_co2, 4, "co2", "%");
    SHOW_WYKRES(a_8, 5, "czujnik 8", "%");
    SHOW_WYKRES(a_temp_komory, 6, "Temp. komory", "st C");
    SHOW_WYKRES(a_cisn_komora, 7, "Ciśn. komory", "mbar");
    SHOW_WYKRES(a_temp_parownik, 8, "Temp. parownika", "st C");

    foreach (auto im, m_imageCheckBox) {
        if (im->isChecked()) {
            testDane.addImage(testWorkDir.absoluteFilePath(im->text()) + QString(".png"));
        }
    }

    QPrinter printer(QPrinter::PrinterResolution);
    QTextDocument * textDocument = new QTextDocument;

    QTextCursor cursor(textDocument);

    textDocument->setHtml(testDane.getBody());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("test.pdf");
    printer.setPageSize(QPageSize(QPageSize::A4));
    qInfo() << printer.paperSize(QPrinter::Point);
    printer.setFullPage(true);
    textDocument->print(&printer);

}
