#ifndef TESTPAGEBASE_H
#define TESTPAGEBASE_H

#include <QWidget>
class QFrame;

namespace Ui {
class TestPageForm;
}
class TestPage;

class TestPageForm : public QWidget
{
    Q_OBJECT
public:
    explicit TestPageForm(QWidget *parent = nullptr);

    void addWidget(TestPage * page);
    TestPage*  widget() const { return page; }
    void setTitle(const QString & title);
    void setSubTitle(const QString & title);
    void setButtonName(const QString & name);
    void isComplete();
    void initializePage();
    QFrame * widgetFrame();
protected slots:
    void click();
signals:
    void clickButton(int id);
public:
    ~TestPageForm();
    int getId() const;
    void setId(int value);
    void disableButton(bool disable);

private:
    Ui::TestPageForm *ui;
    int id;
    TestPage * page;
};


#endif // TESTPAGEBASE_H
