#include "Driving_Motor.h"
#include <Arduino_RouterBridge.h>

#define LEFT_MOTOR_PWM_PIN D5
#define RIGHT_MOTOR_PWM_PIN D6



void RespondToButton() {
  static bool isEnabled = false;
  isEnabled = !isEnabled;

  if (isEnabled) {
    Monitor.println("ACTIVATING MOTORS");
    StartMotors();
  }
  else {
    Monitor.println("DEACTIVATING MOTORS");
    StopMotors();
  }
}

void InitMotors() {
  pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);
}

void StartMotors() {
  int TARGET_VAL = 125;
  analogWrite(LEFT_MOTOR_PWM_PIN, TARGET_VAL);
  analogWrite(RIGHT_MOTOR_PWM_PIN, TARGET_VAL);
}
void StopMotors() {
  analogWrite(LEFT_MOTOR_PWM_PIN, 0);
  analogWrite(RIGHT_MOTOR_PWM_PIN, 0);  
}

int GetRPM(Side motor) {
  int count = 0;
  int previousMillis = 0;
  const int PERIOD = 1000;
  int last_pin_state = LOW;

}

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


