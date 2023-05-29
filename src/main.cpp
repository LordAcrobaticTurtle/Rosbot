
// #include <Rosbot.h>
#include <drivers/RadioInterface.h>
#include <drivers/RGB_LED.h>
#include <drivers/DRV8876.h>
#include <utility/math.h>
#include <drivers/IMU.h>
// Rosbot robot;
#include <Arduino.h>
#include <i2c_device.h>

IMU imu;

bool configure_sensor();

void setup()
{  
    Serial.begin(9600);
    Serial.println("Begin!");
    Wire.begin();
    // master.begin(100 * 1000U);
    // configured = configure_sensor();

    // robot.setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    imu.setup(&Wire);
    while (!imu.m_configured) {
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        imu.setup(&Wire);
    }
    
}

void loop()
{
    imu.update();
    
    // if (configured) {
    //     // Do stuff
    //     uint8_t buffer[6];
    //     uint8_t address = 0x43;
    //     for (int i = 0; i < 6; i++) {
    //         if (!sensor.read(address, &buffer[i], true)) {
    //             Serial.println("ERROR: Failed to read IMU");
    //         }
    //     }
    //     uint16_t gx = buffer[0] << 8 | buffer[1];
    //     uint16_t gy = buffer[2] << 8 | buffer[3];
    //     uint16_t gz = buffer[4] << 8 | buffer[5];

    //     float gxf, gyf, gzf;

    //     gxf = gx / 16.5;
    //     gyf = gy / 16.5;
    //     gzf = gz / 16.5;
    //     Serial.println("gx: " + String(gxf) + "gy: " + String(gyf) + "gz: " + String(gzf));
    //     Serial.println();
    // } else {
    //     Serial.println("Not configured");
    // }
}

// bool configure_sensor() {
//     uint16_t wakeup = 0;
//     if (!sensor.write(0x6B, wakeup, false)) {
//         return false;
//     }
//     uint16_t gyroSenstivity = 0b00001000;
//     if (!sensor.write(0x1B, gyroSenstivity, false)) {
//         return false;
//     }

//     return true;
// }

