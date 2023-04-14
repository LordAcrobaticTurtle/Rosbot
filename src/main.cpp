
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Rosbot.h>
#include <drivers/RadioInterface.h>
#include <drivers/AS5600.h>
#include <drivers/IMU.h>
#include <drivers/RGB_LED.h>
#include <i2c_device.h>
#include <i2c_driver.h>

// IMU imu;
// Rosbot robot;
RadioInterface rx(&Serial1);
RGBLED led(3,4,2,true);

int timer = 0;
int array[3] = {0,0,0};
int loopCounter = 0;
int red, green, blue;

void setup()
{  
    Serial.begin(115200);
    Serial.println("Begin!");
    // Setup I2C comms
    // Wire.begin();
    // Wire.setClock(400000);
    // Wire1.begin();
    rx.setup();
    red = 0;
    green = 0;
    blue = 0;
    // Setup serial lines
    
    // Switch on LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Setup rosbot
    // robot.setup();    
    // imu.setup(&Wire1);
}

void loop()
{
    // robot.tankDrive();
    // imu.update();
    rx.update();
    // rx.printChannels();
    double channels[TX_NUM_CHANNELS];
    rx.getChannelPercentage(channels);

    red = channels[5]*255;
    green = channels[6]*255;
    blue = channels[2]*255;
    
    if (millis() - timer > 100) {
        timer = millis();
        led.mix(red, green, blue);

        loopCounter++;
        if (loopCounter > 2) 
            loopCounter = 0;
        
    }
    Serial.println("");
}



