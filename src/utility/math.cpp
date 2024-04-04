#include <utility/math.h>
#include <iostream>

double floatMap(
    double value, double preMin, double preMax, 
    double postMin, double postMax) 
{
    return (postMax - postMin)/(preMax - preMin) * (value - preMin) + postMin;
}

int clamp(int value, int min, int max) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    }

    return value;

}

Matrix::Matrix(int rows, int cols) {
    
    if (rows >= MAX_ARRAY_SIZE || cols >= MAX_ARRAY_SIZE) {
        numRows = -1;
        numCols = -1;
        return;
    }

    numRows = rows;
    numCols = cols;
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        for (int j = 0 ; j < MAX_ARRAY_SIZE; j++) {
            data[i][j] = 0;
        }
    }
}

bool Matrix::isValid() {
    return (numCols > 0) && (numRows > 0) && (numCols < MAX_ARRAY_SIZE) && (numRows < MAX_ARRAY_SIZE);
}

Matrix Matrix::multiply(Matrix a1, Matrix a2) {
    // Order matters
    // a1 * a2 != a2 * a1

    if (a1.numCols != a2.numRows) {
        return Matrix(-1,-1);
    }

    Matrix output(a2.numCols, a1.numRows);
    for (int i = 0; i < a1.numRows; i++) {
        // Compute result then insert into new matrix.
        // std::cout << "[ ";
        for (int j = 0; j < a2.numCols; j++) {
            for (int k = 0; k < a1.numCols; k++) {
                output.data[i][j] += a1.data[i][k] * a2.data[k][j];
            }
            // std::cout << output.data[i][j] << ", ";
        }
        // std::cout << "]" << std::endl;
    }
    return output;
}

Matrix Matrix::add(Matrix a1, Matrix a2) {

    if (a1.numCols != a2.numCols || a1.numRows != a2.numRows) {
        return Matrix(-1, -1);
    }

    Matrix output(a1.numRows, a1.numCols);

    for (int i = 0; i < a1.numRows; i++) {
        for (int j=0; j < a1.numCols; j++) {
            output.data[i][j] = a1.data[i][j] + a2.data[i][j];
        }
    }

    return output;
}

Matrix Matrix::subtract(Matrix a1, Matrix a2) {
    if (a1.numCols != a2.numCols || a1.numRows != a2.numRows) {
        return Matrix(-1, -1);
    }

    Matrix output(a1.numRows, a1.numCols);

    for (int i = 0; i < a1.numRows; i++) {
        for (int j=0; j < a1.numCols; j++) {
            output.data[i][j] = a1.data[i][j] - a2.data[i][j];
        }
    }

    return output;
}