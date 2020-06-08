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
    int detEnter(vector<double> objfunctbasis) {
        int res = -1;
        double largest = 0;

        for (size_t c = 0; c < objfunctbasis.size(); c++) {
            if (objfunctbasis[c] > largest) {
                largest = objfunctbasis[c];
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
            res = res + (a[h]*b[h]);
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
        bool finished = false;
        size_t iteration = 1;
        int enter, leave, col;
        double t;

        while (!finished && iteration <= 5	) {


            // Get entering variable
            enter = detEnter(objective_function_basis);
            if (enter == -1) { // if none, we're done here
                double z = vectorMultiplication(y, b);
                result = "Optimal value of ";
                result.append(to_string(z));
                result.append(" has been reached\n");
                result.append("Original:");

                for (size_t i = 1; i <= cols; i++) {
                    result.append("\tx");
                    result.append(to_string(i));
                    result.append("=");
                    result.append(to_string(determVar(i,xb,xn,b_basis)));

                }
                result.append("\nSlack:");

                for (size_t i = cols+1; i <= rplusc; i++) {
                    result.append("\tx");
                    result.append(to_string(i));
                    result.append("=");
                    result.append(to_string(determVar(i, xb, xn, b_basis)));

                }
                result.append("\n");

                finished = true;
                break;
            }


            // update d (abarj)
            abasis = a[xn[enter] - 1];
            double vp;
            for (int g = 0; g < (int)iteration-1; g++) {
                col = (int)eta[g][rows];
                vp = abasis[col] / eta[g][col];
                abasis[col] = vp;

                for (size_t i = 0; i < rows; i++) {
                    if (i != col) {
                        abasis[i] -= eta[g][i] * abasis[col];
                    }
                }
            }


            // Determine leaving variable
            leave = leavingRatio(b_basis, abasis);
            if (leave == -1) {
                finished = true;
                break;
            }
            t = b_basis[leave] / abasis[leave];



            // Update eta file (first input is E1 not E0)
            for (size_t i = 0; i < abasis.size(); i++) {
                eta[iteration - 1][i] = abasis[i];
            }
            eta[iteration - 1][rows] = leave;

            swap(xb[leave], xn[enter]); // Swap entering and leaving

            // Update y
            for (size_t i = 0; i < rows; i++) {
                y[i] = objective_function[xb[i] - 1];
            }

            for (int g = iteration - 1; g >= 0; g--) {
                col = (int)eta[g][rows];
                for (size_t i = 0; i < rows; i++) {
                    if (i != col) {
                        y[col] -= eta[g][i] * y[i];
                    }
                }

                y[col] /= eta[g][col];
            }

            // Update bbar
            for (size_t i = 0; i < rows; i++) {
                b_basis[i] -= t * abasis[i];
                if (abs(b_basis[i]) < 0.001) {
                    b_basis[i] = 0;
                }
                if (b_basis[i] < 0) { // Sign constraints
                    finished = true;
                    break;
                }
            }
            b_basis[leave] = t;

            // Update cbar
            for (size_t j = 0; j < cols; j++) {
                objective_function_basis[j] = objective_function[xn[j] - 1] - vectorMultiplication(y, a[xn[j] - 1]);
            }

            // Increase iteration for y and updates and max iterations
            iteration++;
        }

        return result;
    }





};

#endif // MATRIX_H
