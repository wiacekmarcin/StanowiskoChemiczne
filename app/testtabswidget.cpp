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

TestTabsWidget::TestTabsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget)
{
    ui->setupUi(this);
}

#define SETCZUJANAL(A, N) do { Ustawienia::CzujnikAnalogowy temp = ust.getCzujnikAnalogowyUstawienia(A); \
    ui->check_AddPdf_##N->setText(temp.name); \
    ui->unit_1_##N->setText(temp.unit); \
    ui->unit_2_##N->setText(temp.unit); \
    czAnalRatio[A] = temp.convert; \
    czAnalUnit[A] = temp.unit; \
    } while(false);

TestTabsWidget::TestTabsWidget(const ProjectItem &pr, const QString &testName, const Ustawienia & ust, QWidget *parent) :
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
    testDane.setNazwaTestu(testName);
    ui->stackedWidget->setTestData(&testDane);
    qInfo() << __FILE__ << ":" << __LINE__ << "test=" << testDane.getNazwaTestu();

    SETCZUJANAL(a_voc1, 1);
    SETCZUJANAL(a_voc2, 2);
    SETCZUJANAL(a_o2, 3);
    SETCZUJANAL(a_co2, 4);
    SETCZUJANAL(a_8, 5);
    SETCZUJANAL(a_temp_komory, 6);
    SETCZUJANAL(a_cisn_komora, 7);
    SETCZUJANAL(a_temp_parownik, 8);
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

void TestTabsWidget::finishedTest(bool success)
{
    qInfo() << __FILE__ << ":" << __LINE__ << "test=" << testDane.getNazwaTestu();
    if (success) {
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
        QImage c = im.scaledToWidth(900);
        if (c.size().height() > 600) {
            QImage d = c.scaledToHeight(600);
            c = d;
        }
        qDebug() << "Image size=" << c.size();
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
        checkBox->setObjectName(QString::fromUtf8("checkBoxImage")+QString::number(images.size()));
        checkBox->setText(basename);
        ui->gridLayoutCheckbox->addWidget(checkBox, images.size() , 0);

        QSpacerItem * im_sp = new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum);
        ui->gridLayoutCheckbox->addItem(im_sp, images.size(), 1);

        QLineEdit * le = new QLineEdit(this);
        le->setObjectName(QString::fromUtf8("lineEditImage")+QString::number(images.size()));
        ui->gridLayoutCheckbox->addWidget(le, images.size(), 2);

        ImagesOpisType imtype;
        imtype.box = checkBox;
        imtype.lineedit = le;
        m_imageCheckBox << imtype;
        images << fileNamePng;

    }

}

#define SHOW_WYKRES(A, N, O1, O2)     pdf.setWykresVisible(A, ui->check_AddPdf_##N->isChecked(), \
    ui->dbmin_##N->value(), ui->dbmax_##N->value(), czAnalRatio[A], ui->page_##N->value(), O1, O2, czAnalUnit[A])

void TestTabsWidget::on_pbCreateRaport_clicked()
{
    PdfCreator pdf(testDane);
    pdf.clearImage();

    SHOW_WYKRES(a_voc1, 1, "Wykres wartości stężenia czujnika VOC1:", "Evikon E2638, etanol %LEL");
    SHOW_WYKRES(a_voc2, 2, "Wykres wartości stężenia czujnika VOC2:", "Evikon E2638, etanol %LEL");
    SHOW_WYKRES(a_o2, 3, "Wykres wartości stężenia czujnika O2:", "Evikon E2638, tlen 0-25%");
    SHOW_WYKRES(a_co2, 4, "Wykres wartości stężenia czujnika CO2:", "Vaisala GMP251, 0-20%");
    SHOW_WYKRES(a_8, 5, "Wykres wartości stężenia czujnika wirtualnego:", "Przeliczenia wskazań z VOC1, wg stałej wprowadzonej przez użytkownika");
    SHOW_WYKRES(a_cisn_komora, 7, "Wykres wartości ciśnienia wewnątrz komory:", "WIKA A-10, 0-2,5 bar abs");
    SHOW_WYKRES(a_temp_parownik, 8, "Wykres wartości temperatury parownika:", "Shimaden SR91, czujnik Pt100");
    SHOW_WYKRES(a_temp_komory, 6, "Wykres wartości temperatury wewnątrz komory:", "Shimaden SD17, termopara typ K, fi3mm");

    foreach (auto im, m_imageCheckBox) {
        if (im.box->isChecked()) {
            pdf.addImage(testWorkDir.absoluteFilePath(im.box->text()) + QString(".png"), im.lineedit->text());
        }
    }

    QPrinter printer(QPrinter::PrinterResolution);
    QTextDocument * textDocument = new QTextDocument;

    textDocument->setHtml(pdf.getBody());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("test.pdf");
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setFullPage(true);
    textDocument->print(&printer);

}

QDataStream & operator<<(QDataStream & ds, const TestTabsWidget & item)
{
    ds << item.testDane << item.testWorkDir.absolutePath() << item.testWorkDirName << item.images;
    //ds << (*(item.ui->stackedWidget));

    //foreach(const QString imS, item.images) {
    //    QImage i(item.testWorkDir.absoluteFilePath(imS));
    //    ds << i;
    //}
    return ds;
}

QDataStream & operator>>(QDataStream & ds, TestTabsWidget & item)
{
    QString sTestWorkDir;
    QStringList files;
    ds >> item.testDane >> sTestWorkDir >> item.testWorkDirName >> files;
    //item.ui->stackedWidget->initFromFile(item.testDane)

    item.testWorkDir = QDir(sTestWorkDir);
    foreach(const QString imS, files) {
        QFile f(item.testWorkDir.absoluteFilePath(imS));
        if (f.exists())
            item.images << imS;
    }

    //    QImage i;
    //    i << ds;
    //
    //}


    return ds;
}

