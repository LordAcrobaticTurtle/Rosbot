
#include <Rosbot.h>
#include <comms/comms.h>

#include <i2c_device.h>

std::shared_ptr<Comms> comms;
std::shared_ptr<Rosbot> robot;
std::shared_ptr<RadioInterface> rx;

void mainloop();

void setup()
{  
    Serial.begin(9600);
    Serial.println("Begin!");
    Serial4.begin(9600);
    Master.begin(100 * 1000U);
    robot = std::make_shared<Rosbot>();
    rx = std::make_shared<RadioInterface>(&Serial1);
    comms = std::make_shared<Comms>(robot, rx);
    robot->setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    
    
    
}
 
void loop()
{
    // Check for button press. 
    // Debounce button press. 
    // Change state
    // Serial4.println("BEANS");
    // robot.run();
    
}


void mainloop() {
    // Still has access to rosbot here
    // Can write a scheduling class and affect it
}

