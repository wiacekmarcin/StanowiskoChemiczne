#ifndef FORMULARZDIALOGBUTTONBOX_H
#define FORMULARZDIALOGBUTTONBOX_H

#include <QWidget>
#include <QDialogButtonBox>

class QAbstractButton;

class FormularzDialogButtonBox : public QDialogButtonBox
{
    Q_OBJECT

public:
    FormularzDialogButtonBox(QWidget *prevTab, short step, short max, QWidget * parent = NULL);

public slots:
    void setValid(bool v);

signals:
    void reset();
    void next();
    void prev();
    void abort();


private:
    QAbstractButton * b_ok;
    QAbstractButton * b_prev;
    QAbstractButton * b_next;
    QAbstractButton * b_reset;
    QAbstractButton * b_abort;
};

#endif // FORMULARZDIALOGBUTTONBOX_H
