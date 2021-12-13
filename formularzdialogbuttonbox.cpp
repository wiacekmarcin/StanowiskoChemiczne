#include "formularzdialogbuttonbox.h"

#include <QAbstractButton>
#include <QPushButton>

FormularzDialogButtonBox::FormularzDialogButtonBox(short step, short max, QWidget * parent)
 : QDialogButtonBox(QDialogButtonBox::NoButton, Qt::Horizontal, parent)
    ,b_ok(new QPushButton("Zakończ", this))
    ,b_prev(new QPushButton("Wstecz", this))
    ,b_next(new QPushButton("Dalej", this))
    ,b_reset(new QPushButton(QString::fromUtf8("Wyczyść"), this))
    ,b_abort(new QPushButton("Przerwij", this))
{

    //addButton(b_reset, QDialogButtonBox::ResetRole);

    if (step > 1)
        addButton(b_prev, QDialogButtonBox::DestructiveRole);

    if (step == max) {
        addButton(b_ok, QDialogButtonBox::AcceptRole);
    } else {
        addButton(b_next, QDialogButtonBox::AcceptRole);
    }


    addButton(b_abort, QDialogButtonBox::RejectRole);

    //adjustSize();
}

void FormularzDialogButtonBox::setValid(bool v)
{
    b_ok->setEnabled(v);
    b_next->setEnabled(v);
}
