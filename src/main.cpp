
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Rosbot.h>
#include <drivers/RadioInterface.h>
#include <drivers/AS5600.h>
#include <drivers/IMU.h>
#include <drivers/RGB_LED.h>
#include <drivers/DRV8876.h>
#include <utility/math.h>
#include <i2c_device.h>
#include <i2c_driver.h>

Rosbot robot;

void setup()
{  
    Serial.begin(115200);
    Serial.println("Begin!");
    robot.setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
    robot.update();
}

