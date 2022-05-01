#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include <QThread>
#include <QHash>

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

    void on_actionUstawieniaOgolne_triggered();

    void on_actionZapisz_triggered();

    void on_actionOtw_rz_triggered();

protected:
    void changeSelectedTest();
    void setActionText();
    void disableNowyTest(bool dis);

private:
    Ui::GlowneOkno *ui;
    Urzadzenia * urzadzenia;
    Ustawienia & settings;

    QHash<QTreeWidgetItem*, ProjectItem> projekty;
    QHash<QTreeWidgetItem*, TestTabsWidget*> testy;
    QHash<QTreeWidgetItem*, QTreeWidgetItem*> mapTesty;
    QHash<QTreeWidgetItem*, QList<QTreeWidgetItem*> > mapProjektTesty;
    QTreeWidgetItem* selectedProject;
    QTreeWidgetItem* selectedTest;

    QAction * act_wykresy[Ustawienia::maxCzujekAnal];
    QAction * act_wyzwal[Ustawienia::maxCzujekAnal];
    QSignalMapper *signalMapper;
    bool showDebugDlg;

    QThread thReadDigAnal;
    QThread thDozownik;

    QThread thWykresy;
    QThread thTest;

};


#endif // GLOWNEOKNO_H
