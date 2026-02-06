#include "Driving_Motor.h"


// void setup() {
//   currentState = CONNECT;

//   // InitBluetooth();
//   Monitor.begin();
//   delay(1000);
//   Monitor.println("Ballzooka powered on.");




//   pinMode(D7, INPUT);
// }

// int count = 0;
// int previousMillis = 0;
// int PERIOD = 1000;
// int last_pin_state = LOW;

// void loop() {
//   // check for failures

//   if (millis() - previousMillis >= PERIOD) {
//     previousMillis = millis();
//     Monitor.println((count / 3) * 60);
//     count = 0;
//   }

//   int pin_state = digitalRead(D7);
//   if (pin_state == HIGH && last_pin_state == LOW) {  // rising edge
//       count++;
//   }
//   last_pin_state = pin_state;


