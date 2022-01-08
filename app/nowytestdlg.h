#ifndef NOWYTESTDLG_H
#define NOWYTESTDLG_H

#include <QDialog>
#include <QMap>
#include <QString>

namespace Ui {
class NowyTestDlg;
}

class NowyTestDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NowyTestDlg(QWidget *parent = 0);
    ~NowyTestDlg();
    QString getName() const;
    int getDozownik() const;
    QString getLuquid() const;
    double getVolume() const;
    QString getIngition() const;
    QString getIngitionExt() const;

private slots:
    void on_nameTest_textChanged(const QString &arg1);
    void on_cbDozownik_currentIndexChanged(int index);
    void on_cbCiecz_currentIndexChanged(int index);
    void on_iloscCieczy_valueChanged(double arg1);
    void on_cbZaplon_currentIndexChanged(int index);
    void on_cbenergiaIskry_currentIndexChanged(int index);

protected:
    void checkValid();
private:
    Ui::NowyTestDlg *ui;
    bool valid;
    QMap<QString, int> maxVal;
};

#endif // NOWYTESTDLG_H
