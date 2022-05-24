#include "testdialog.h"
#include "ui_testdialog.h"

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);
}

TestDialog::~TestDialog()
{
    delete ui;
}

const QString &TestDialog::getActTime() const
{
    return actTime;
}

void TestDialog::setActTime(const QString &newActTime)
{
    actTime = newActTime;
    ui->time->setText(newActTime);
}

const QString &TestDialog::getActCisnie() const
{
    return actCisnie;
}

void TestDialog::setActCisnie(const QString &newActCisnie)
{
    actCisnie = newActCisnie;
    ui->cisnienie->setText(newActCisnie);
}

const QString &TestDialog::getDownLevel() const
{
    return downLevel;
}

void TestDialog::setDownLevel(const QString &newDownLevel)
{
    downLevel = newDownLevel;
    ui->down->setText(newDownLevel);
}

const QString &TestDialog::getUpLevel() const
{
    return upLevel;
}

void TestDialog::setUpLevel(const QString &newUpLevel)
{
    upLevel = newUpLevel;
    ui->up->setText(newUpLevel);
}

const QString &TestDialog::getMaxTime() const
{
    return maxTime;
}

void TestDialog::setMaxTime(const QString &newMaxTime)
{
    maxTime = newMaxTime;
    ui->maxtime->setText(newMaxTime);
}

const QString &TestDialog::getFirstTimeWait() const
{
    return firstTimeWait;
}

void TestDialog::setFirstTimeWait(const QString &newFirstTimeWait)
{
    firstTimeWait = newFirstTimeWait;
    ui->fisrtTIme->setText(newFirstTimeWait);

}

const QString &TestDialog::getSecondTimeWait() const
{
    return secondTimeWait;
}

void TestDialog::setSecondTimeWait(const QString &newSecondTimeWait)
{
    secondTimeWait = newSecondTimeWait;
    ui->secondTime->setText(newSecondTimeWait);
}

const QString &TestDialog::getAllHisterezy() const
{
    return allHisterezy;
}

void TestDialog::setAllHisterezy(const QString &newAllHisterezy)
{
    allHisterezy = newAllHisterezy;
    ui->maxHisterez->setText(newAllHisterezy);
}

const QString &TestDialog::getNumHisterezy() const
{
    return numHisterezy;
}

void TestDialog::setNumHisterezy(const QString &newNumHisterezy)
{
    numHisterezy = newNumHisterezy;
    ui->histerez->setText(newNumHisterezy);
}

const QString &TestDialog::getZadaneCisnie() const
{
    return zadaneCisnie;
}

void TestDialog::setZadaneCisnie(const QString &newZadaneCisnie)
{
    zadaneCisnie = newZadaneCisnie;
    ui->cisnie_zad->setText(newZadaneCisnie);
}

const QString &TestDialog::getBezczynnoscPompy() const
{
    return bezczynnoscPompy;
}

void TestDialog::setBezczynnoscPompy(const QString &newBezczynnoscPompy)
{
    bezczynnoscPompy = newBezczynnoscPompy;
    ui->pompanorun->setText(newBezczynnoscPompy);
}
