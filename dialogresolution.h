#ifndef DIALOGRESOLUTION_H
#define DIALOGRESOLUTION_H

#include <QDialog>

namespace Ui {
class DialogResolution;
}

class DialogResolution : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResolution(QWidget *parent = nullptr);
    ~DialogResolution();

    int resX;
    int resY;
    int resZ;

private slots:
    void on_Confirm_released();
    void on_ResX_valueChanged(int val);
    void on_ResY_valueChanged(int val);
    void on_ResZ_valueChanged(int val);

private:
    Ui::DialogResolution *ui;
};

#endif // DIALOGRESOLUTION_H
