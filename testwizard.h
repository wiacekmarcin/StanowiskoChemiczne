#ifndef TESTWIZARD_H
#define TESTWIZARD_H

#include <QWizard>

class TestWizard : public QWizard
{
    Q_OBJECT

public:
    explicit TestWizard(QWidget *parent = 0);
    ~TestWizard();
private slots:
    void changePage(int id);
};

#endif // TESTWIZARD_H
