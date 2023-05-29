// #pragma once
// // void encoderSetup();
// // void encoderCallbackR();
// // void encoderCallbackL();



// // // Right motor

// volatile int prevR = 0;
// volatile int currR = 0;
// volatile bool motordirR;
// volatile float rpmR;
// volatile int countR = 0;


// // // Left motor
// volatile int prevL = 0;
// volatile int currL = 0;
// volatile bool motordirL;
// volatile float rpmL = 0;


// // void encoderSetup() {
// //     pinMode(enc1_c1, INPUT_PULLUP);
// //     pinMode(enc1_c2, INPUT_PULLUP);
// //     pinMode(enc2_c1, INPUT_PULLUP);
// //     pinMode(enc2_c2, INPUT_PULLUP);
// //     attachInterrupt(enc1_c1, encoderCallbackR, RISING);
// //     attachInterrupt(enc2_c1, encoderCallbackL, RISING);
// // }


// // void encoderCallbackR() {
// //     // Access global variables
// //     // right.c1_callback();
// //     motordirR = digitalRead(enc1_c2);

// //     if (motordirR == HIGH) {
// //         countR--;
// //     } else if (motordirR == LOW) {
// //         countR++;
// //     }

// //     currR = micros();
// //     if (prevR < currR) {
// //         float rev = currR - prevR;
// //         rev = 1.0 / rev;
// //         rev *= 1000000;
// //         rev *= 60;
// //         rev /= GEARING;
// //         rev /= ENCODERMULT;
// //         rpmR = rev;
// //     }
// //     prevR = currR;
// //     // led.toggleGreen();
// // }

// // void encoderCallbackL() {
// //     // Access global variables
// //     // right.c1_callback();
// //     motordirR = digitalRead(enc1_c2);
// //     if (prevR < currR) {
// //         float rev = currR - prevR;
// //         rev = 1.0 / rev;
// //         rev *= 1000000;
// //         rev *= 60;
// //         rev /= GEARING;
// //         rev /= ENCODERMULT;
// //     }
// //     prevR = currR;
    
// // }