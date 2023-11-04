
#include "utility/math.h"

#include "gtest/gtest.h"


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
    Matrix a1(3,3);
    Matrix a2(3,3);
    Matrix output = Matrix::multiply(a1,a2);

    EXPECT_EQ(output.numCols, 3);
    EXPECT_EQ(output.numRows, 3);
}

TEST_F(TestMath, test_matrix_math_output_rectangle_matrix) {
    Matrix a1(1,3);
    a1.data[0][0] = 1;
    a1.data[0][1] = 2;
    a1.data[0][2] = 3;
    
    Matrix a2(3,1);
    a2.data[0][0] = 1;
    a2.data[1][0] = 2;
    a2.data[2][0] = 3;

    Matrix output = Matrix::multiply(a1,a2);

    EXPECT_EQ(output.numCols, 1);
    EXPECT_EQ(output.numRows, 1);
    EXPECT_EQ(output.data[0][0], 14);
}

TEST_F(TestMath, test_matrix_math_output_rectnagle_flipped) {
    Matrix a1(1,3);
    a1.data[0][0] = 1;
    a1.data[0][1] = 2;
    a1.data[0][2] = 3;
    
    Matrix a2(3,1);
    a2.data[0][0] = 1;
    a2.data[1][0] = 2;
    a2.data[2][0] = 3;

    Matrix output = Matrix::multiply(a2,a1);
    
    EXPECT_EQ(output.numCols, 3);
    EXPECT_EQ(output.numRows, 3);

    for (int i = 0 ; i < output.numCols; i++) {
        for (int j=0; j < output.numRows; j++) {
            EXPECT_EQ
        }
    }
}