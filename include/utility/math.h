#pragma once
#include <math.h>

#define MAX_ARRAY_SIZE 10

double floatMap(double value, double preMin, double preMax, 
                double PostMin, double PostMax);

int clamp(int value, int min, int max);

struct Matrix {
    Matrix(int rows, int cols);
    int numCols;
    int numRows;
    int data[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];

    bool isValid();
    static Matrix multiply(Matrix a1, Matrix a2);
    static Matrix add(Matrix a1, Matrix a2);
    static Matrix subtract(Matrix a1, Matrix a2);
};


