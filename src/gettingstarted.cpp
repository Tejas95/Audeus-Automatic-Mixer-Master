#include "gettingstarted.h"
#include "ui_gettingstarted.h"
#include "noise_1.h"

GettingStarted::GettingStarted(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GettingStarted)
{
    ui->setupUi(this);
}

GettingStarted::~GettingStarted()
{
    delete ui;
}

void GettingStarted::on_PBNext_clicked()
{
    this->close();

    Noise_1 noise1;
    noise1.setModal(true);
    noise1.exec();
}
