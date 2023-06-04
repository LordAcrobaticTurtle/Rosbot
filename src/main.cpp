
#include <Rosbot.h>
#include <drivers/RadioInterface.h>
#include <drivers/RGB_LED.h>
#include <drivers/DRV8876.h>
#include <utility/math.h>

Rosbot robot;

void setup()
{  
    Serial.begin(9600);
    Serial.println("Begin!");
    Master.begin(100 * 1000U);
    robot.setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    
}


void loop()
{
    robot.update();
}

