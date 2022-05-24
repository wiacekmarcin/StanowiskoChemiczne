#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

    const QString &getActTime() const;
    void setActTime(const QString &newActTime);

    const QString &getActCisnie() const;
    void setActCisnie(const QString &newActCisnie);

    const QString &getDownLevel() const;
    void setDownLevel(const QString &newDownLevel);

    const QString &getUpLevel() const;
    void setUpLevel(const QString &newUpLevel);

    const QString &getMaxTime() const;
    void setMaxTime(const QString &newMaxTime);

    const QString &getFirstTimeWait() const;
    void setFirstTimeWait(const QString &newFirstTimeWait);

    const QString &getSecondTimeWait() const;
    void setSecondTimeWait(const QString &newSecondTimeWait);

    const QString &getAllHisterezy() const;
    void setAllHisterezy(const QString &newAllHisterezy);

    const QString &getNumHisterezy() const;
    void setNumHisterezy(const QString &newNumHisterezy);

    const QString &getZadaneCisnie() const;
    void setZadaneCisnie(const QString &newZadaneCisnie);

    const QString &getBezczynnoscPompy() const;
    void setBezczynnoscPompy(const QString &newBezczynnoscPompy);

private:
    Ui::TestDialog *ui;
    QString actTime;
    QString actCisnie;
    QString downLevel;
    QString upLevel;
    QString maxTime;
    QString firstTimeWait;
    QString secondTimeWait;
    QString allHisterezy;
    QString numHisterezy;
    QString zadaneCisnie;
    QString bezczynnoscPompy;

};

#endif // TESTDIALOG_H
