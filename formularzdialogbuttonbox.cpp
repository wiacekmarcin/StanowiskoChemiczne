#include "formularzdialogbuttonbox.h"

#include <QAbstractButton>
#include <QPushButton>

FormularzDialogButtonBox::FormularzDialogButtonBox(QWidget * prevTab, short step, short max, QWidget * parent)
 : QDialogButtonBox(Qt::Horizontal, parent)
    ,b_ok(new QPushButton("Zakończ", this))
    ,b_prev(new QPushButton("Wstecz", this))
    ,b_next(new QPushButton("Dalej", this))
    ,b_reset(new QPushButton(QString::fromUtf8("Wyczyść"), this))
    ,b_abort(new QPushButton("Przerwij", this))
{

    QWidget::setTabOrder(prevTab, b_reset);
    addButton(b_reset, QDialogButtonBox::ResetRole);

    addButton(b_prev, QDialogButtonBox::DestructiveRole);
    QWidget::setTabOrder(b_reset, b_prev);

    addButton(b_abort, QDialogButtonBox::RejectRole);
    QWidget::setTabOrder(b_prev, b_abort);

    addButton(b_ok, QDialogButtonBox::AcceptRole);
    QWidget::setTabOrder(b_abort, b_ok);

    addButton(b_next, QDialogButtonBox::AcceptRole);
    QWidget::setTabOrder(b_ok, b_next);


    b_prev->setEnabled(step != 1);
    b_next->setEnabled(step != max);
    b_ok->setEnabled(step == max);

    //b_reset->setShortcut();

}

void FormularzDialogButtonBox::setValid(bool v)
{
    b_ok->setEnabled(v);
    b_next->setEnabled(v);
}
