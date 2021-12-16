#include "formularzdialogbuttonbox.h"

#include <QAbstractButton>
#include <QPushButton>
#include<QAction>

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

    QAction * a_reset = new QAction(b_reset);
    a_reset->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_C));
    b_reset->addAction(a_reset);
    connect(a_reset, &QAction::trigger, this, &FormularzDialogButtonBox::reset);

    QAction * a_prev = new QAction(b_prev);
    a_prev->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_W));
    b_prev->addAction(a_prev);
    connect(a_prev, &QAction::trigger, this, &FormularzDialogButtonBox::prev);

    QAction * a_next = new QAction(b_next);
    a_next->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_D));
    b_next->addAction(a_next);
    connect(a_next, &QAction::trigger, this, &FormularzDialogButtonBox::next);

    QAction * a_abort = new QAction(b_abort);
    a_abort->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_P));
    b_abort->addAction(a_abort);
    connect(a_abort, &QAction::trigger, this, &FormularzDialogButtonBox::abort);

    QAction * a_ok = new QAction(b_ok);
    a_ok->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_Z));
    b_ok->addAction(a_ok);
    connect(a_ok, &QAction::trigger, this, &FormularzDialogButtonBox::next);

}

void FormularzDialogButtonBox::setValid(bool v)
{
    b_ok->setEnabled(v);
    b_next->setEnabled(v);
}
