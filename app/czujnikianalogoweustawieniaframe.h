#ifndef CZUJNIKIANALOGOWEUSTAWIENIAFRAME_H
#define CZUJNIKIANALOGOWEUSTAWIENIAFRAME_H

#include "ustawienia.h"

#include <QFrame>

class CzujnikAnalogowyUstawieniaWidget;
class QGridLayout;

/**
 * @brief The CzujnikiAnalogoweUstawieniaFrame class
 * Klasa przechowująca widget zawierający ustawieanie czujników analagowych
 * @param czujniki - tablica na pojedyńczy widget zawierający ustawienia dla 1 czujnika
 * @param gridLayout - layout do którego dodaje poszczególne elementy
 */
class CzujnikiAnalogoweUstawieniaFrame : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief CzujnikiAnalogoweUstawieniaFrame konstruktor
     * @param parent - wskaźnika na rodzica
     */
    explicit CzujnikiAnalogoweUstawieniaFrame(QWidget *parent = 0);

    /**
     * Desktruktor
     */
    ~CzujnikiAnalogoweUstawieniaFrame();

    /**
     * @brief setUstawienia - ustawia wartości z konfiguracji
     * @param sett
     */
    void setUstawienia(const UserPrivilige & user, const Ustawienia & sett);

    void saveData(Ustawienia & ust);

private slots:
    void updateCzujnik();

protected:
    void createOneElement(int id, const QString &titleBox)    ;
private:
    CzujnikAnalogowyUstawieniaWidget * czujniki[Ustawienia::maxCzujekAnal];
    QGridLayout * gridLayout;
};

#endif // CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
