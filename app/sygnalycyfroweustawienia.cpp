#include "sygnalycyfroweustawienia.h"
#include "ui_sygnalycyfroweustawienia.h"

SygnalyCyfroweUstawienia::SygnalyCyfroweUstawienia(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SygnalyCyfroweUstawienia)
{
    ui->setupUi(this);
}

SygnalyCyfroweUstawienia::~SygnalyCyfroweUstawienia()
{
    delete ui;
}
