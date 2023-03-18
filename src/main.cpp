
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Rosbot.h>
#include <drivers/RadioInterface.h>
#include <drivers/AS5600.h>
#include <drivers/IMU.h>
#include <i2c_device.h>
#include <i2c_driver.h>

IMU imu;
// Rosbot robot;

void setup()
{  
    // Setup I2C comms
    Wire.begin();
    // Wire.setClock(400000);
    Wire1.begin();

    // Setup serial lines
    Serial.begin(115200);
    Serial.println("Begin!");
 
    // Switch on LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Setup rosbot
    // robot.setup();    
    imu.setup(&Wire1);
}

void loop()
{
    // robot.tankDrive();
    imu.update();
}



