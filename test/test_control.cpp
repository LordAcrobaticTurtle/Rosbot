#include "control/pid_controller.h"


class FixturePIDController {
    public:
        void SetUp() {}
        void TearDown() {}
};


class TestPIDController : public PIDController {
    public:
        TestPIDController() = default;

};


// TEST_F (FixturePIDController, test_response_is_capped) {
//     PIDParams params;
//     params.bounds[0] = -1;
//     params.bounds[1] = 1;
//     params.currValue = 0.5;
//     params.target = 0.7;
//     params.dt = 0.001; // seconds
//     params.
// }