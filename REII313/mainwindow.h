#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Num_Vars_SpinBox_valueChanged(int arg1);

    void on_Num_Constraints_SpinBox_valueChanged(int arg1);

    void on_Input_data_clicked();

    void on_Optimise_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
