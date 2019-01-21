#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

    void OpenVolumeData(int x, int y, int z);

private slots:
	void on_action_Quit_triggered();
	void on_action_Open_triggered();

    void on_BrightnessBox_valueChanged(double val);
    void on_MinBox_valueChanged(double val);
    void on_MaxBox_valueChanged(double val);

    void on_Opaque_released();
    void on_Transparent_released();

private:
	Ui::MainWindow *ui;
    void sendParams();
};

#endif // MAINWINDOW_H
