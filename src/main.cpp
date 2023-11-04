
#include <Rosbot.h>
#include <comms/comms.h>
#include <drivers/encoder_N20.h>
#include <i2c_device.h>
#include <drivers/DRV8876.h>

// std::shared_ptr<Comms> comms;
// std::shared_ptr<Rosbot> robot;
// std::shared_ptr<RadioInterface> rx;

EncoderN20 enc1(6,7);
EncoderN20 enc2(8,9);
DRV8876 driver(23, 21, 14, -1, 20);
void mainloop();

void setup()
{  
    Serial.begin(9600);
    Serial.println("Begin!");
    Serial4.begin(9600);
    Master.begin(100 * 1000U);
    // robot = std::make_shared<Rosbot>();
    // rx = std::make_shared<RadioInterface>(&Serial1);
    // comms = std::make_shared<Comms>(robot, rx);
    // robot->setup();
    // rx->setup();
    enc1.setup();
    enc2.setup();
    
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    
}
 
void loop()
{
    // driver.setThrottle(60);
    EncoderResult res = enc2.readRPMwithStruct();
    if (res.option == VALID) {
        Serial.println("RPM: " + String(res.RPM) + ", current: " + String(driver.readCurrentAnalog()));
    }
    // Serial.println();
}


void mainloop() {
    // Still has access to rosbot here
    // Can write a scheduling class and affect it
}

