#include <utility/math.h>

double floatMap(
    double value, double preMin, double preMax, 
    double postMin, double postMax) 
{
    return (postMax - postMin)/(preMax - preMin) * (value - preMin) + postMin;
}


bool Matrix::isValid() {
    return (numCols > 0) && (numRows > 0);
}

Matrix Matrix::multiply(Matrix a1, Matrix a2) {
    // Order matters
    // a1 * a2 != a2 * a1

    if (a1.numCols != a2.numRows) {
        return Matrix(-1,-1);
    }
    
    Matrix output(a2.numCols, a1.numRows);
    for (int i = 0; i < a2.numCols; i++) {
        // Compute result then insert into new matrix.
        for (int j = 0; j < a1.numRows; j++) {
            for (int k = 0; k < a1.numCols; k++) {
                output.data[i][j] += a1.data[i][k] * a2.data[k][j];
            }
        }
    }

    return output;
}