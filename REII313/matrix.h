#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>
#include <string>
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

    void setObjective(size_t j,double val){

            //if j < number of vars get cost equation values from user
            //put J+1 values into an xn array that represents x1,x2,x3....xn, these are the variables excluding slack vars
            if (j < cols) {
                objective_function[j]=val;
                objective_function_basis[j] = objective_function[j];
                xn[j] = j + 1;
            }
            //if j=n or greater put J+1 values into xb array thaat represents the slack variables vector xn,xn+1,xn+2......xm
            else {
                xb[j - cols] = j + 1;
            }

    }

    void setConstraintsMatrix(int i,int j, double val){
                //rows and cols are switched to make extraction easier
                if(j != cols)
                    a[j][i]=val;
                else
                    b[i]=val;

            //adding of slack variable
            a[i + cols][i] = 1;
            ab[i][i] = 1;

        b_basis = b;
    }
    //determines entering variable regarding the objective function
    int detEnter(vector<double> cbasis) {
        int res = -1;
        double largest = 0;

        for (size_t c = 0; c < cbasis.size(); c++) {
            if (cbasis[c] > largest) {
                largest = cbasis[c];
                res = c;
            }
        }

        return res;
    }
    double determVar(int num, vector<double> xb, vector<double> xn, vector<double> bbasis) {

        for (size_t i = 0; i < xn.size(); i++) {
            if (xn[i] == num)
                return 0;
        }
        for (size_t j = 0; j < xb.size(); j++) {
            if (xb[j] == num) {
                if (abs(bbasis[j]) > 0.00001) {
                    return bbasis[j];
                }
                else {
                    return 0;
                }
            }
        }

        return -1;
    }

    double vectorMultiplication(vector<double> a, vector<double> b) {
        double res = 0;
        for (size_t h = 0; h < b.size(); h++) {
            res += a[h]*b[h];
        }

        return res;
    }

    int leavingRatio(vector<double> b,vector<double> abasis){
        bool first = true;
        int res = -1;
        double smallest = 0;
        double ratio;


        for (size_t j = 0; j < b.size(); j++) {
            if (abasis[j] > 0) {
                ratio = b[j] / abasis[j];


                if (ratio < smallest || first) {
                    first = false;
                    res = j;
                    smallest = ratio;
                }
            }
        }


        return res;
    }

    string Simplex_Solve(){
        string result;


        return result;
    }


};

#endif // MATRIX_H
