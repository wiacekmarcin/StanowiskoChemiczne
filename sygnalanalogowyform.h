#ifndef SYGNALANALOGOWYFORM_H
#define SYGNALANALOGOWYFORM_H

#include <QWidget>

namespace Ui {
class SygnalAnalogowyForm;
}

class SygnalAnalogowyForm : public QWidget
{
    Q_OBJECT

public:
    explicit SygnalAnalogowyForm(QWidget *parent = 0);
    ~SygnalAnalogowyForm();
    QString name() const;
    QString unit() const;
    QString ratio() const;

private:
    Ui::SygnalAnalogowyForm *ui;
};

#endif // SYGNALANALOGOWYFORM_H
