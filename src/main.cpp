
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
    // stepperL.setMaxSpeed(stepperMaxSpeed);
    // stepperL.setAcceleration(stepperMaxAccel);
    // // stepperL.setSpeed(200);
    // stepperR.setMaxSpeed(stepperMaxSpeed);
    // stepperR.setAcceleration(stepperMaxAccel);
    // stepperR.setSpeed(200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);
    Serial.println("Begin!");
    // Wire.begin();
    // rx.setup();
    Wire.begin();
    Wire1.begin();
    // encL.setup();
    // encR.setup();
    
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



