#ifndef CREATETESTWIZARD_H
#define CREATETESTWIZARD_H

#include <QStackedWidget>
#include <QMap>
#include <QList>
#include "testdata.h"

class TestPage;
class CreateTestWizard : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CreateTestWizard(QWidget *parent = 0);
    void setTestData(const TestData & dt);
    ~CreateTestWizard();

    void setField(const QString & key, const QVariant & val);
    QVariant field(const QString & key) const;
    void addPage(TestPage * page);
protected:
    void init();

    void initialPage();
private:
    QMap<QString,  QVariant> values;
    QList<TestPage*> pages;
    short selectedId;

};

#endif // CREATETESTWIZARD_H
