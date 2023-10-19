
#include <Rosbot.h>
#include <drivers/BleTransceiver.h>
#include <comms/comms_layer.h>

Rosbot robot;

BLETransceiver blComms;

void mainloop();

void setup()
{  
    Serial.begin(9600);
    Serial.println("Begin!");
    Serial4.begin(9600);
    Master.begin(100 * 1000U);
    robot.setup();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    
    blComms.init(&Serial4, 9600);
    
}
 
void loop()
{
    // Check for button press. 
    // Debounce button press. 
    // Change state
    // Serial4.println("BEANS");
    robot.update();

    // Receive bytes from serial port
    char buffer[256];
    byte bytesRead = blComms.readBytes(buffer, 256);

    Packet pkt;
    PacketSerializer::deserialize(buffer, pkt);
    
}


void mainloop() {
    // Still has access to rosbot here
    // Can write a scheduling class and affect it
}

