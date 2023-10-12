#include <drivers/N20Encoder.h>
#include <utility/error_codes.h>


volatile int enc1Count = 0;
volatile int enc2Count = 0;

// volatile int prevR = 0;
// volatile int currR = 0;
// volatile bool motordirR;
// volatile float rpmR;
volatile float RPM = 0;
volatile uint32_t lastA = 0;
volatile bool direction = 0;

elapsedMicros enc1Timer;
elapsedMicros enc2Timer;

volatile uint32_t enc1PrevRiseTime = 0;
volatile uint32_t enc1RiseTime = 0;

volatile int enc1PrevCount = 0;
volatile int enc2PrevCount = 0;
volatile byte prevEncState = 0;

void encoder1_c1_callback() {
    // Disable interrupts
    bool isAHigh = digitalReadFast(enc1_c1);
    bool isBHigh = digitalReadFast(enc1_c2);

    // Serial.println("A: " + String(isAHigh) + ", B: " + String(isBHigh));
    // Anti-clockwise 
    if ((isAHigh && !isBHigh) || (!isAHigh && isBHigh)) {
        enc1Count++;
    }
    // Clockwise
    else if ((isAHigh && isBHigh) || (!isAHigh && !isBHigh)) {
        enc1Count--;
    }

    
    // if (isAHigh) {
    //     // Set current time in millis 
    //     enc1PrevCount = enc1Count;
    //     enc1PrevRiseTime = enc1RiseTime;
    //     enc1RiseTime = millis();
    // }
    // if (isAHigh) {
    //     isBHigh ? enc1Count-- : enc1Count++;
    // }
    // else {
    //     isBHigh ? enc1Count++ : enc1Count--;
    // }
}

// B channel on encoder
void encoder1_c2_callback() {
    cli();
    bool isAHigh = digitalReadFast(enc1_c1); // A
    bool isBHigh = digitalReadFast(enc1_c2); // B

    // Anti-clockwise
    if ((isBHigh && isAHigh) || (!isBHigh && !isAHigh)) {
        enc1Count++;
    }
    // Clockwise
    else if ((isBHigh && !isAHigh) || (!isBHigh && isAHigh)) {
        enc1Count--;
    }
    // if (isBHigh) {
    //     // B is high
    //     isAHigh ? enc1Count++ : enc1Count--;
    // }
    // else {
    //     // B is low
    //     isAHigh ? enc1Count-- : enc1Count++;
    // } 

    sei();
}

void encoder2_c1_callback() {
    cli();
    bool direction = digitalRead(enc2_c2);
    // Update encoder appropriately
    direction? enc2Count++ : enc2Count--;
    sei();
}

void encoder2_c2_callback() {
    cli();
    bool direction = digitalRead(enc2_c1);
    
    // Update encoder appropriately
    direction? enc2Count++ : enc2Count--;
    sei();
}

N20Encoder::N20Encoder() {}

int N20Encoder::setup(  void (*c1_Callback)(void), void (*c2_Callback)(void), 
                        volatile int *count,
                        uint32_t pinC1, uint32_t pinC2) {
    
    if (c1_Callback == NULL) {
        return ErrorCodes::ERROR;
    }

    m_pinC1 = pinC1;
    m_pinC2 = pinC2;
    
    pinMode(m_pinC1, INPUT_PULLUP);
    pinMode(m_pinC2, INPUT_PULLUP);
    
    attachInterrupt(m_pinC1, c1_Callback, CHANGE);
    attachInterrupt(m_pinC2, c2_Callback, CHANGE);
    m_count = count;
    *m_count = 0;
}

void N20Encoder::update() {
    float count;
    cli();
    memcpy(&count, (float *)&RPM, sizeof(float));
    sei();


    // Calculate revolutions
    // Grab previous encoder count
    // Can be a positive/negative number
    // Grab current encoder count

    int encoderDiff = enc1Count - enc1PrevCount;
    // Find difference -> Change in position 

    // Zero prev encoder count
    enc1PrevCount = enc1Count;
    // Divide by PPR. Divide by gear ratio
    double revolutionsPerSecond = (double) encoderDiff / PPR;

    double dt = enc1RiseTime - enc1PrevRiseTime;
    
    dt /= 1000;
    revolutionsPerSecond /= GEARING;
    revolutionsPerSecond /= dt;
    // Calculate rev/s
    // Multiply dt by 1000
    // Divide by dt

    
    Serial.println("Encoder RPM: " + String(revolutionsPerSecond) + ", " + String(encoderDiff) + ", " + String(dt));
    // Do computation for velocity here
}


