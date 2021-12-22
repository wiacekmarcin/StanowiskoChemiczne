#ifndef TESTPAGEBASE_H
#define TESTPAGEBASE_H

#include <QWidget>


namespace Ui {
class TestPageForm;
}

class TestPageForm : public QWidget
{
    Q_OBJECT
public:
    explicit TestPageForm(QWidget *parent = nullptr);

    void addWidget(QWidget * page);
    void setTitle(const QString & title);
    void setSubTitle(const QString & title);
public:
    ~TestPageForm();
private:
    Ui::TestPageForm *ui;
};


#endif // TESTPAGEBASE_H
