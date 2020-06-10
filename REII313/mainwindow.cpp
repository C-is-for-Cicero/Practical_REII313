#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simplex.h"


int num_vars;
int num_cons;

Simplex mat;

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
    ui->A_Matrix->setColumnCount(arg1+1);
    ui->Objective_Function->setColumnCount(arg1);
    ui->Objective_Function->setRowCount(1);

    QTableWidgetItem *header1 = new QTableWidgetItem();

    header1->setText("b");

    for(int i=0;i<(ui->Objective_Function->columnCount());++i){
            ui->Objective_Function->setHorizontalHeaderItem(i, new QTableWidgetItem);
            Q_ASSUME(ui->Objective_Function->model()->setHeaderData(i,Qt::Horizontal,QStringLiteral("X_%1").arg(i+1)));
    }

    for(int i=0;i<(ui->A_Matrix->columnCount()-1);++i){
            ui->A_Matrix->setHorizontalHeaderItem(i, new QTableWidgetItem);
            Q_ASSUME(ui->A_Matrix->model()->setHeaderData(i,Qt::Horizontal,QStringLiteral("X_%1").arg(i+1)));
    }

    ui->A_Matrix->setHorizontalHeaderItem(ui->A_Matrix->columnCount()-1, header1);

//    for(int i=0;i<ui->A_Matrix->columnCount();i++){
//        if(i==ui->A_Matrix->columnCount()-1){
//            ui->A_Matrix->setHorizontalHeaderItem(i,header1);
//            break;
//        }
//        ui->A_Matrix->setHorizontalHeaderItem(i,header2);
//    }
}

void MainWindow::on_Num_Constraints_SpinBox_valueChanged(int arg1)
{
    num_cons=arg1;
    ui->A_Matrix->setRowCount(arg1);


}

void MainWindow::on_Input_data_clicked()
{
    mat=Simplex(num_cons,num_vars);
    double val;

    for(unsigned int i=0;i<mat.cols;i++){
        val=ui->Objective_Function->item(0,i)->text().toDouble();
        mat.setObjective(i,val);
    }

    for(unsigned int i = mat.cols;i<mat.rplusc;i++){
        mat.setObjective(i,val);
    }

    for(int i=0;i<mat.rows;i++){
        for (int j=0;j<(mat.cols+1);j++){
            val = ui->A_Matrix->item(i,j)->text().toDouble();
            mat.setConstraintsMatrix(i,j,val) ;
        }
    }

    ui->test->setText(QString::number( mat.xb[0] ));

}

void MainWindow::on_Optimise_clicked()
{
    string s;
    s=mat.Simplex_Solve();
    QString qstr = QString::fromStdString(s);
    ui->test->setText(qstr);
}
