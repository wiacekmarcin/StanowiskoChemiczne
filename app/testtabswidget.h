#ifndef TESTTABSWIDGET_H
#define TESTTABSWIDGET_H

#include <QWidget>
#include <QDir>
#include <QStringList>

#include "projectitem.h"
#include "testdata.h"
#include "ustawienia.h"
namespace Ui {
class TestTabsWidget;
}

class CreateTestWizard;
class QCheckBox;

class TestTabsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestTabsWidget(QWidget *parent = 0);
    TestTabsWidget(const ProjectItem & pr, QWidget * parent);
    ~TestTabsWidget();
    void setActive();
    CreateTestWizard * createTestWizard() const;

public slots:
    void finishedTest(bool success);
private slots:
    void on_pbAddImage_clicked();

    void on_pbCreateRaport_clicked();

private:
    Ui::TestTabsWidget *ui;
    ProjectItem projekt;
    TestData testDane;

    QDir testWorkDir;
    QString testWorkDirName;
    QStringList images;
    QList<QCheckBox*> m_imageCheckBox;
};

#endif // TESTTABSWIDGET_H
