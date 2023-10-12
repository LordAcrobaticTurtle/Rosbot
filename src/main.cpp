
#include <Rosbot.h>

Rosbot robot;


void setup()
{  
    Serial.begin(9600);
    Serial.println("Begin!");
    Serial4.begin(9600);
    Master.begin(100 * 1000U);
    robot.setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    
}
 
void loop()
{
    // Check for button press. 
    // Debounce button press. 
    // Change state
    // Serial4.println("BEANS");
    robot.update();
}

