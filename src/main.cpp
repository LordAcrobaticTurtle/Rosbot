
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <RadioInterface.h>
#include <Rosbot.h>
#include <AS5600.h>
#include <i2c_device.h>
#include <i2c_driver.h>


// Define a stepper and the pins it will use
// AccelStepper stepperL(AccelStepper::DRIVER, 12,14); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
// AccelStepper stepperR(AccelStepper::DRIVER, 2,3);
// RadioInterface rx;

// AS5600 encL(&Wire);
// AS5600 encR(&Wire1);
Rosbot robot;
// int stepperMaxSpeed = 400; //steps per second
// int stepperMaxAccel = 80; // steps per second per second

void setup()
{  
    Wire.begin();
    Wire1.begin();
    Serial.begin(115200);
    Serial.println("Begin!");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    robot.setup();
    
}

void loop()
{
    
    robot.tankDrive();
    // Write something to switch I2C lines. 
    // 
    // rx.update();
    // unsigned int encLStatus = encL.getAngle();
    // unsigned int encRStatus = encR.getAngle();
    
    // Serial.print("encL: " + String(encLStatus) + ", encR: " + String(encRStatus));
    // Serial.println("");
    delay(10);
}



