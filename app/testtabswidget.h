#ifndef TESTTABSWIDGET_H
#define TESTTABSWIDGET_H

#include <QWidget>
#include <QDir>
#include <QStringList>
#include <QDataStream>

#include "projectitem.h"
#include "testdata.h"
#include "ustawienia.h"
namespace Ui {
class TestTabsWidget;
}

class QCheckBox;
class QLineEdit;

typedef struct _imagesOpis {
    QCheckBox * box;
    QLineEdit * lineedit;
} ImagesOpisType;

class CreateTestWizard;


class TestTabsWidget : public QWidget
{
    Q_OBJECT

public:
    //explicit TestTabsWidget(QWidget *parent = 0);
    TestTabsWidget(const QString & testName, const Ustawienia &ust, QWidget * parent);
    ~TestTabsWidget();
    void setActive();
    CreateTestWizard * createTestWizard() const;
    QString getTestName() const;

    void finishedTest(const ProjectItem & projekt);
    void initfinishedTest();

signals:
    void processImageSignal(QStringList filenames);

    

    friend QDataStream & operator<<(QDataStream & ds, const TestTabsWidget & item);
    friend QDataStream & operator>>(QDataStream & ds, TestTabsWidget & item);

private slots:
    void on_pbAddImage_clicked();
    void on_pbCreateRaport_clicked();

    void processImageSlot(QStringList filenames);
    void processVideSlot(QStringList filenames, QString dirdest);

    void tbOpen_clicked();
    void tbbegin_clicked();
    void tbminus100_clicked();
    void tbminus50_clicked();
    void tbminus10_clicked();
    void tbminus5_clicked();
    void tbminus1_clicked();
    void tbplay_clicked();
    void tbplus1_clicked();
    void tbplus10_clicked();
    void tbplus5_clicked();
    void tbplus50_clicked();
    void tbplus100_clicked();
    void tbend_clicked();

    void pbAddImage_video2_clicked();

private:
    Ui::TestTabsWidget *ui;
    TestData testDane;
    const Ustawienia & ust;

    QDir testWorkDir;
    QString testWorkDirName;
    QStringList images;
    QList<ImagesOpisType> m_imageCheckBox;

    QMap<analogIn, double> czAnalRatio;
    QMap<analogIn, QString> czAnalUnit;

    bool playVideo;
};

QDataStream & operator<<(QDataStream & ds, const TestTabsWidget & item);
QDataStream & operator>>(QDataStream & ds, TestTabsWidget & item);

#endif // TESTTABSWIDGET_H
