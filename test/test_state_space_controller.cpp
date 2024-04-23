#include "control/state_space_control.h"
#include "gtest/gtest.h"


class TestStateSpaceController : public ::testing::Test {
    public:
        TestStateSpaceController() : controller(4,1) {}

        void SetUp () override {

        }

        void TearDown () override {

        }
    
    public:
        StateSpaceControl controller;
};


TEST_F(TestStateSpaceController, test_control_signal) {
    Matrix gains(1,4);
    gains.data[0][0] = 1.0f;
    gains.data[0][1] = 1.0f;
    gains.data[0][2] = 1.0f;
    gains.data[0][3] = 1.0f;


    Matrix state(4,1);
    Matrix desiredState(4,1);

    state.data[0][0] = 1.0f;
    state.data[1][0] = 1.0f;
    state.data[2][0] = 1.0f;
    state.data[3][0] = 1.0f;

    desiredState.data[0][0] = 1.0f;
    desiredState.data[1][0] = 2.0f;
    desiredState.data[2][0] = 3.0f;
    desiredState.data[3][0] = 4.0f;

    controller.setGainMatrix(gains);

    float u = controller.calculateControlSignal(desiredState, state);

    EXPECT_EQ(u, -6.0f);
}



