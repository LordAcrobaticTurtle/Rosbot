


#include <interfaces/imu_interface.h>
#include <MPU6050_6Axis_MotionApps20.h>

class catsMPU6050 : public ImuInterface {
    public:
        catsMPU6050 ();

        virtual int readGyroRates (vector3D &rates)         override;
        virtual int readOrientation (vector3D &orientation) override;
        virtual int readAccel (vector3D &accel)             override;
        virtual int readTemperature (float *temp)           override;
        virtual int readMagnetField (vector3D &field)       override;
        virtual int readImuData (ImuData &data)             override;
        
        // Updates all data objects
        virtual int run () override;

    private:

        MPU6050 m_imu;
        // MPU control/status vars
        bool dmpReady = false;  // set true if DMP init was successful
        uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
        uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        uint16_t fifoCount;     // count of all bytes currently in FIFO
        uint8_t fifoBuffer[64]; // FIFO storage buffer

        // orientation/motion vars
        Quaternion q;           // [w, x, y, z]         quaternion container
        VectorInt16 aa;         // [x, y, z]            accel sensor measurements
        VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
        VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
        VectorFloat gravity;    // [x, y, z]            gravity vector
        float euler[3];         // [psi, theta, phi]    Euler angle container
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

        const int m_interruptPin = 15;

};