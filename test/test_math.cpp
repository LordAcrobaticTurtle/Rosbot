
#include "utility/math.h"

#include "gtest/gtest.h"
#include <stdio.h>

Matrix a1_3x3(3,3);
Matrix a2_3x3(3,3);
Matrix a1_1x3(1,3);
Matrix a2_3x1(3,1);

Matrix multiplicationCorrectResult(3,3);
Matrix correctResult(3,3);

void setupTestForAddition() {

    a1_3x3.data[0][0] = 1;
    a1_3x3.data[0][1] = 2;
    a1_3x3.data[0][2] = 3;
    a1_3x3.data[1][0] = 2;
    a1_3x3.data[1][1] = 4;
    a1_3x3.data[1][2] = 6;
    a1_3x3.data[2][0] = 3;
    a1_3x3.data[2][1] = 6;
    a1_3x3.data[2][2] = 9;

    a2_3x3.data[0][0] = 1;
    a2_3x3.data[0][1] = 2;
    a2_3x3.data[0][2] = 3;
    a2_3x3.data[1][0] = 2;
    a2_3x3.data[1][1] = 4;
    a2_3x3.data[1][2] = 6;
    a2_3x3.data[2][0] = 3;
    a2_3x3.data[2][1] = 6;
    a2_3x3.data[2][2] = 9;

    correctResult.data[0][0] = 1*2;
    correctResult.data[0][1] = 2*2;
    correctResult.data[0][2] = 3*2;
    correctResult.data[1][0] = 2*2;
    correctResult.data[1][1] = 4*2;
    correctResult.data[1][2] = 6*2;
    correctResult.data[2][0] = 3*2;
    correctResult.data[2][1] = 6*2;
    correctResult.data[2][2] = 9*2;
}

void setupTestForMultiplication() {
    a1_1x3.data[0][0] = 1;
    a1_1x3.data[0][1] = 2;
    a1_1x3.data[0][2] = 3;

    a2_3x1.data[0][0] = 1;
    a2_3x1.data[1][0] = 2;
    a2_3x1.data[2][0] = 3;

    correctResult.data[0][0] = 1;
    correctResult.data[0][1] = 2;
    correctResult.data[0][2] = 3;
    correctResult.data[1][0] = 2;
    correctResult.data[1][1] = 4;
    correctResult.data[1][2] = 6;
    correctResult.data[2][0] = 3;
    correctResult.data[2][1] = 6;
    correctResult.data[2][2] = 9;
}

void setupTestForSubtraction() {
    a1_3x3.data[0][0] = 1;
    a1_3x3.data[0][1] = 2;
    a1_3x3.data[0][2] = 3;
    a1_3x3.data[1][0] = 2;
    a1_3x3.data[1][1] = 4;
    a1_3x3.data[1][2] = 6;
    a1_3x3.data[2][0] = 3;
    a1_3x3.data[2][1] = 6;
    a1_3x3.data[2][2] = 9;

    a2_3x3.data[0][0] = 1;
    a2_3x3.data[0][1] = 2;
    a2_3x3.data[0][2] = 3;
    a2_3x3.data[1][0] = 2;
    a2_3x3.data[1][1] = 4;
    a2_3x3.data[1][2] = 6;
    a2_3x3.data[2][0] = 3;
    a2_3x3.data[2][1] = 6;
    a2_3x3.data[2][2] = 9;

    correctResult.data[0][0] = 0;
    correctResult.data[0][1] = 0;
    correctResult.data[0][2] = 0;
    correctResult.data[1][0] = 0;
    correctResult.data[1][1] = 0;
    correctResult.data[1][2] = 0;
    correctResult.data[2][0] = 0;
    correctResult.data[2][1] = 0;
    correctResult.data[2][2] = 0;
}

void compareMatrices(Matrix computation, Matrix correct) {
    
    EXPECT_EQ(computation.numCols, correct.numCols);
    EXPECT_EQ(computation.numRows, correct.numRows);

    for (int i=0; i < correct.numRows; i++) {
        for (int j=0; j < correct.numCols; j++) {
            EXPECT_EQ(correct.data[i][j], computation.data[i][j]);
        }
    }

    EXPECT_EQ(computation.isValid(), correct.isValid());
    
}

class TestMath : public ::testing::Test {
    public:
        void SetUp() {}
        void TearDown() {}
};

/**
 * @brief Test the matrix math produces the correct output for a 
 * Square matrix input
 */
TEST_F(TestMath, test_matrix_math_correct_dimensions_output) {
    Matrix output = Matrix::multiply(a1_3x3,a2_3x3);

    EXPECT_EQ(output.numCols, 3);
    EXPECT_EQ(output.numRows, 3);
}

TEST_F(TestMath, test_matrix_multiplication_output_rectangle_matrix) {
    setupTestForMultiplication();
    Matrix output = Matrix::multiply(a1_1x3, a2_3x1);
    Matrix result(1,1);
    result.data[0][0] = 14;

    compareMatrices(output, result);
}

TEST_F(TestMath, test_matrix_multiplication_output_3x1_x_1x3) {
    setupTestForMultiplication();

    Matrix output = Matrix::multiply(a2_3x1, a1_1x3);
    
    compareMatrices(output, correctResult);

}

TEST_F(TestMath, test_matrix_is_not_valid) {
    Matrix test(11,11);
    EXPECT_EQ(test.isValid(), false);
    EXPECT_EQ(test.numCols, -1);
    EXPECT_EQ(test.numRows, -1);
}

TEST_F(TestMath, test_matrix_addition) {
    setupTestForAddition();

    Matrix output = Matrix::add(a1_3x3, a2_3x3);

    compareMatrices(output, correctResult);
}

TEST_F(TestMath, test_matrix_subtraction) {
    setupTestForSubtraction();
    
    Matrix output = Matrix::subtract(a1_3x3, a2_3x3);
    compareMatrices(output, correctResult);
}

TEST_F(TestMath, test_matrix_control_operations) {
    Matrix K(1,4);
    Matrix state(4,1);
    Matrix desired(4,1);
    K.data[0][0] = -1.0;
    K.data[0][1] = -1.1116;
    K.data[0][2] = 7.7973;
    K.data[0][3] = 0.9166;


    state.data[0][0] = 1.0;
    state.data[1][0] = 1.0;
    state.data[2][0] = 1.0;
    state.data[3][0] = 1.0;

    desired.data[0][0] = 2.0;
    desired.data[1][0] = 2.0;
    desired.data[2][0] = 2.0;
    desired.data[3][0] = 2.0;

    Matrix u(1,1);
    u = Matrix::multiply(K, Matrix::subtract(desired,state));
    
    EXPECT_FLOAT_EQ(u.data[0][0], 6.6023);
    EXPECT_FLOAT_EQ(u.data[1][0], 0);
    EXPECT_FLOAT_EQ(u.data[0][1], 0);
    EXPECT_FLOAT_EQ(u.data[1][1], 0);
}

