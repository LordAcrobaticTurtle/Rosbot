
typedef struct PIDValues {
    float target;
    float currValue;
    float kp;
    float ki;
    float kd;
    float prevError;
    float errorSum;
} PIDValues;

class PID {
    public:
        static float computeResponse(PIDValues pidVals);

};