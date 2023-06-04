
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Rosbot.h>
#include <drivers/RadioInterface.h>
#include <drivers/AS5600.h>
#include <drivers/IMU.h>
#include <drivers/RGB_LED.h>
#include <drivers/DRV8876.h>
#include <utility/math.h>

// Rosbot robot;
IMU imu;
RGBLED led(3,4,2,true);

void setup()
{  
    Serial.begin(115200);
    Serial.println("Begin!");
    Master.begin(100 * 1000U);
    led.switchRedOn();    
    imu.setup(Master);
    led.switchGreenOn();
    // robot.setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
    // robot.update();
    imu.update();
}

