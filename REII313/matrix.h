#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>

using namespace std;

#define MAXITER 100
class Matrix{
public:

    size_t rows;//number of constraint equations
    size_t cols;//number of variables
    size_t rplusc;//size of the square matrix(when slack vars are added)
    vector<double> objective_function; //objective or cost function
    vector<double> objective_function_basis;//basis of obj function
    vector<vector <double> > a;//constraints matrix
    vector<double> abasis;//basis of constraints matrix excluding b
    vector<vector <double> > ab;//ab matrix
    vector<double> b;//b vactor
    vector<double> b_basis;//basis of b
    vector<double> y;//objective function
    vector<double> xb;//basis vector
    vector<double> xn;//non basis vector
    vector<vector<double>> e;
    vector<vector<double>> eta;//Eta matrix


    Matrix(){

    }

    //constructor initialiser list
    Matrix(size_t r,size_t c)
          :rows(r),
          cols(c),
          rplusc(r+c),
          objective_function(r+c),
          objective_function_basis(c),
          a(r+c, vector<double>(r)),
          abasis(r),
          ab(r, vector<double>(r)),
          b(r),
          b_basis(r),
          y(r),
          xb(r),
          xn(c),
          e(r, vector<double>(r)),
          eta(MAXITER, vector<double>(r + 1)){}




};

#endif // MATRIX_H
