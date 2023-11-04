#pragma once
#include <math.h>

#define MAX_ARRAY_SIZE 10

double floatMap(double value, double preMin, double preMax, 
                double PostMin, double PostMax);


struct Matrix {
    Matrix(int rows, int cols) : numRows(rows), numCols(cols) {
        for (int i = 0; i < numCols; i++) {
            for (int j = 0 ; j < numRows; j++) {
                data[i][j] = 0;
            }
        }
    }
    int numCols;
    int numRows;
    int data[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];

    bool isValid();
    static Matrix multiply(Matrix a1, Matrix a2);
};
