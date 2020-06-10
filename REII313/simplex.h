#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <vector>
#include <iostream>
#include <string>
#include "matrix.h"
using namespace std;
#define MAXITER 100

class Simplex:public Matrix{
public:
    vector<vector<double>> e;
    vector<vector<double>> eta;

    Simplex(){

    }

    Simplex(size_t r,size_t c)
        : Matrix(r,c),
          e(r, vector<double>(r)),
          eta(MAXITER, vector<double>(r + 1)){}

    int Determine_Enter(vector<double> objfunctbasis) {
        int res = -1;
        double largest = 0;
        size_t i=0;
        while (i<objfunctbasis.size()){
            if (objfunctbasis[i] > largest) {
                largest = objfunctbasis[i];
                res = i;
            }
            i++;
        }

        return res;
    }

    double Determine_Variable(int num, vector<double> xb, vector<double> xn, vector<double> bbasis) {
    size_t i=0,j=0;

    while(i<xn.size()){
        if (xn[i] == num)
            return 0;
        i++;
    }

    while(j<xb.size()){
        if (xb[j] == num) {
            if (abs(bbasis[j]) > 0.00001) {
                return bbasis[j];
            }
            else {
                return 0;
            }
        }
        j++;
    }



        return -1;
    }

    double vectorMultiplication(vector<double> a, vector<double> b) {
        double res = 0;
        size_t i=0;
        while(i<b.size()){
            res = res + (a[i]*b[i]);
            i++;
        }

        return res;
    }

    int leavingRatio(vector<double> b,vector<double> abasis){
        bool first = true;
        int res = -1;
        double smallest = 0;
        double ratio;
        size_t j=0;

        while(j<b.size()){
            if (abasis[j] > 0) {
                ratio = b[j] / abasis[j];


                if (ratio < smallest || first) {
                    first = false;
                    res = j;
                    smallest = ratio;
                }
            }

            j++;
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
             enter = Determine_Enter(objective_function_basis);
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
                     result.append(to_string(Determine_Variable(i,xb,xn,b_basis)));

                 }
                 result.append("\nSlack:");

                 for (size_t i = cols+1; i <= rplusc; i++) {
                     result.append("\tx");
                     result.append(to_string(i));
                     result.append("=");
                     result.append(to_string(Determine_Variable(i, xb, xn, b_basis)));

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


             for (size_t i = 0; i < rows; i++) {
                 b_basis[i] -= t * abasis[i];
                 if (abs(b_basis[i]) < 0.001) {
                     b_basis[i] = 0;
                 }
                 if (b_basis[i] < 0) { // Sign constraints
                     result="Linear program is not feasible";
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

#endif // SIMPLEX_H
