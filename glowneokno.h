#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include "projectitem.h"
#include "testdata.h"
#include "testtabswidget.h"
namespace Ui {
class GlowneOkno;
}

class  QTreeWidgetItem;
class TestTabWidget;

class GlowneOkno : public QMainWindow
{
    Q_OBJECT

public:
    explicit GlowneOkno(QWidget *parent = 0);
    ~GlowneOkno();

private slots:
    void on_actionUstawienia_sygna_w_triggered();
    void on_actionNowy_projekt_triggered();
    void on_actionNowy_Test_triggered();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

protected:
    void changeSelectedTest();

private:
    Ui::GlowneOkno *ui;
    QMap<QTreeWidgetItem*, ProjectItem> projekty;
    QMap<QTreeWidgetItem*, TestTabsWidget*> testy;
    QMap<QTreeWidgetItem*, QTreeWidgetItem*> mapTesty;
    QTreeWidgetItem* selectedProject;
    QTreeWidgetItem* selectedTest;
};


#endif // GLOWNEOKNO_H
