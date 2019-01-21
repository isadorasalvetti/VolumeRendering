#include <QFileDialog>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogresolution.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
	 QApplication::quit();
}

void MainWindow::on_action_Open_triggered()
{
    DialogResolution resolutionWindow(this);
    resolutionWindow.setModal(true);
    if (resolutionWindow.exec() == QDialog::Accepted)
        OpenVolumeData(resolutionWindow.resX, resolutionWindow.resY, resolutionWindow.resZ);
}

void MainWindow::OpenVolumeData(int x, int y, int z){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open RAW"), ".", tr("*.raw"));
    sendParams();
    ui->openGLWidget->loadMesh(filename, x, y, z);
}

void MainWindow::on_BrightnessBox_valueChanged(double val){
    sendParams();
}

void MainWindow::on_MinBox_valueChanged(double val){
    sendParams();
}

void MainWindow::on_MaxBox_valueChanged(double val){
    sendParams();
}

void MainWindow::sendParams(){
    ui->openGLWidget->updateParameters(ui->BrightnessBox->value(), ui->MinBox->value(), ui->MaxBox->value());
    ui->openGLWidget->update();
}

void MainWindow::on_Opaque_released(){
    ui->openGLWidget->changeShader(OPAQUE);
    sendParams();
}

void MainWindow::on_Transparent_released(){
    ui->openGLWidget->changeShader(TRANSPARENT);
    sendParams();
}
