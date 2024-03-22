
#include <Rosbot.h>
#include <comms/comms.h>
#include <drivers/encoder_N20.h>
#include <drivers/DRV8876.h>
#include <utility/timing.h>

std::shared_ptr<Comms> comms;
std::shared_ptr<Rosbot> robot;
std::shared_ptr<RadioInterface> rx;
// std::shared_ptr<DRV8876> motor;
// EncoderN20 enc1(6,7);
// EncoderN20 enc2(8,9);
// DRV8876 driver(23, 21, 14, -1, 20);

void mainloop();
void setupMainloop();

void setup()
{  
    Serial.begin(115200);
    Serial.println("Begin!");
    Serial4.begin(115200);
    // Master.begin(400 * 1000U);
    setupMainloop();
    
    // driver.wakeup(true);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    
}

void setupMainloop() {
    robot = std::make_shared<Rosbot>();
    robot->setup();
    
    comms = std::make_shared<Comms>(robot);
    
    Serial.print("Start");
}
 
int i = 0;
void loop()
{
    long int start = micros();
    mainloop();
    long int end = micros();

    static FrequencyTimer loopTimer(HZ_200_MICROSECONDS);


    // if (loopTimer.checkEnoughTimeHasPassed()) {
    //     Serial.print("Loop time: ");
    //     Serial.println(end-start);
    // }

    
}


void mainloop() { 

    static FrequencyTimer commsTimer(HZ_50_MICROSECONDS);

    // 50 times a second is not a lot of data. 
    if (commsTimer.checkEnoughTimeHasPassed()) {
        comms->run();
    }
    
    robot->run();
}

    // static FrequencyTimer timer(HZ_100_MICROSECONDS);

    // if (!timer.checkEnoughTimeHasPassed()) {
    //     return;
    // }

    // enc2.run();
    // long int position = enc2.readPosition();

    // PIDParams params;
    // params.bounds[0] = -1;
    // params.bounds[1] = 1;
    // params.dt = 0.01;
    // params.kp = 0.01;
    // params.target = 0;
    // params.currValue = position;

    // double response = PIDController::computeResponse(params);
    // int PWMResponse = response * 255.0;
    // Serial.println(String(PWMResponse) + ", " + String(position));
    // driver.setThrottle(-PWMResponse);