#include "dialogresolution.h"
#include "ui_dialogresolution.h"

DialogResolution::DialogResolution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogResolution)
{
    ui->setupUi(this);
}

DialogResolution::~DialogResolution()
{
    delete ui;
}

void DialogResolution::on_Confirm_released(){
    resX = ui->ResX->value();
    resY = ui->ResY->value();
    resZ = ui->ResZ->value();
    accept();
}
