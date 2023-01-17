
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <RadioInterface.h>
#include <Rosbot.h>
#include <AS5600.h>
#include <i2c_device.h>
#include <i2c_driver.h>

Rosbot robot;

void setup()
{  
    // Setup I2C comms
    Wire.begin();
    Wire1.begin();

    // Setup serial lines
    Serial.begin(115200);
    Serial.println("Begin!");

    // Switch on LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Setup rosbot
    robot.setup();    
}

void loop()
{
    robot.tankDrive();
}



