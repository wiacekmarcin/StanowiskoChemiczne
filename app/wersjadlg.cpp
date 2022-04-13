#include "wersjadlg.h"
#include "ui_wersjadlg.h"
#include "version.h"
#include <QString>

#include "version.h"
#include "build_defs.h"

// want something like: 1.4.1432.2234

const char completeVersion[] =
{
    /*
    VERSION_MAJOR_INIT,
    '.',
    VERSION_MINOR_INIT,
    '-', 'V', '-',*/
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    '-',
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    '-',
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    'T',
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    ':',
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    ':',
    BUILD_SEC_CH0, BUILD_SEC_CH1,
    '\0'
};


WersjaDlg::WersjaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WersjaDlg)
{
    ui->setupUi(this);
    ui->compile_version->setText(QString::number(VERSION_MAJOR) + QString(".") + QString::number(VERSION_MINOR));
    ui->compile_data->setText(QString(completeVersion));
}

WersjaDlg::~WersjaDlg()
{
    delete ui;
}
