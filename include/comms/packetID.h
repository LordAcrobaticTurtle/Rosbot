#include <stdio.h>

enum PacketID {
    BEGIN = 1,           // Command program sends, robot begins mainloop and starts doing its main function. At time of writing that means balancing
    STANDBY,         // Command program sends, robot exits mainloop and awaits further instruction.
    ESTOP,           // Command program sends from button. True or false is encoded in data body. The drive disable pin on motor controller must be on.
    REQUEST,         // Contains the requested packet encoded into its data
    STATE,           // command program requests, mbed side sends. Or mbed just sends periodically 
    ESTIMATE_BIAS,   // Perform bias estimate
    LED_CHANGE,     // Command program sends, r,g,b values contained in data body
    CALIBRATION_MODE,
    NUM_PACKET_IDS
};


namespace commsPacket {
    
    struct Begin {
        bool doEnable;
    };

    struct Standby {
        bool doStandby;
    };

    struct ESTOP {
        bool doStop;
    };

    struct Request {
        PacketID requestedID;
    };
    
    struct State {
        float eulerXYZ[3]; 
        float current[2]; // left, right
        float velocity[2]; // left, right

        // String toString() {
        //     char buffer[128];
        //     sprintf(buffer, "eulerXYZ: %f, %f, %f \ncurrent (L, R): %f, %f\nvelocity (L, R): %f, %f", 
        //             eulerXYZ[0], eulerXYZ[1], eulerXYZ[2], current[0], current[1], velocity[0], velocity[1]);
        //     return String(buffer);
        // }
    };

    struct Error {
        int errorCode;
    };

    struct CalibrationMode {
        bool isCalibrationEnabled;
    };

}


