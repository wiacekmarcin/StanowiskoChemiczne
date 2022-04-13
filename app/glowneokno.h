#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include "projectitem.h"

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
    void dozownikTest();

    void resizeEvent(QResizeEvent * event);
    void on_actionUstawienia_triggered();

    void changeTestName(const QString &name);
    void finishedTest(bool success);

    void on_actionWersja_triggered();

protected:
    void changeSelectedTest();
    void setActionText();
    void disableNowyTest(bool dis);

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
