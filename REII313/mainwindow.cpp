#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrix.h"

int num_vars;
int num_cons;

Matrix mat;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Num_Vars_SpinBox_valueChanged(int arg1)
{
    num_vars=arg1;
    ui->A_Matrix->setColumnCount(arg1);
    ui->Objective_Function->setColumnCount(arg1);
    ui->Objective_Function->setRowCount(1);
}

void MainWindow::on_Num_Constraints_SpinBox_valueChanged(int arg1)
{
    num_cons=arg1;
    ui->A_Matrix->setRowCount(arg1);
    ui->b_vector->setRowCount(arg1);
    ui->b_vector->setColumnCount(1);

}

void MainWindow::on_Input_data_clicked()
{
    mat=Matrix(num_cons,num_vars);
}
