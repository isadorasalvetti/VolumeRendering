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
    accept();
}

void DialogResolution::on_ResX_valueChanged(int val){
    resX = val;
}

void DialogResolution::on_ResY_valueChanged(int val){
    resY = val;
}

void DialogResolution::on_ResZ_valueChanged(int val){
    resZ = val;
}
