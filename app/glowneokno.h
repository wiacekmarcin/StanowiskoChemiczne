#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include "projectitem.h"
#include "testdata.h"
#include "testtabswidget.h"
#include "ustawienia.h"
namespace Ui {
class GlowneOkno;
}

class QTreeWidgetItem;
class TestTabWidget;
class QSignalMapper;
class Urzadzenia;



class GlowneOkno : public QMainWindow
{
    Q_OBJECT

public:
    explicit GlowneOkno(Ustawienia & ust, Urzadzenia *urzadzenia, QWidget *parent = 0);
    ~GlowneOkno();

private slots:
    void on_actionUstawienia_sygna_w_triggered();
    void on_actionNowy_projekt_triggered();
    void on_actionNowy_Test_triggered();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void wybierzCzujke(int id);
    void showIO();
    void dozownikTest();
    void valueChanged(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);
    void resizeEvent(QResizeEvent * event);
    void on_actionUstawienia_triggered();

signals:
    void analogValueChanged(int id, double val);
protected:
    void changeSelectedTest();
    void setActionText();

private:
    Ui::GlowneOkno *ui;
    Urzadzenia * urzadzenia;
    Ustawienia & settings;

    QMap<QTreeWidgetItem*, ProjectItem> projekty;
    QMap<QTreeWidgetItem*, TestTabsWidget*> testy;
    QMap<QTreeWidgetItem*, QTreeWidgetItem*> mapTesty;
    QTreeWidgetItem* selectedProject;
    QTreeWidgetItem* selectedTest;

    QAction * act_wykresy[Ustawienia::maxCzujekAnal];
    QAction * act_wyzwal[Ustawienia::maxCzujekAnal];
    QSignalMapper *signalMapper;
    bool showDebugDlg;

};


#endif // GLOWNEOKNO_H
