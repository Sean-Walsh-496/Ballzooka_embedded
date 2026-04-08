#include "Driving_Motor.h"
#include <Arduino_RouterBridge.h>

#define LEFT_MOTOR_PWM_PIN D5
#define RIGHT_MOTOR_PWM_PIN D6
#define LEFT_MOTOR_HALL_PIN D7
#define RIGHT_MOTOR_HALL_PIN D8

volatile unsigned long leftPulsePeriod = 0;
volatile unsigned long rightPulsePeriod = 0;
volatile unsigned long leftLastPulseTime = 0;
volatile unsigned long rightLastPulseTime = 0;

void leftMotorISR() {
    unsigned long now = micros();
    leftPulsePeriod = now - leftLastPulseTime;
    leftLastPulseTime = now;
}
void rightMotorISR() {
    unsigned long now = micros();
    rightPulsePeriod = now - rightLastPulseTime;
    rightLastPulseTime = now;
}

void RespondToButton() {
  static bool isEnabled = false;
  isEnabled = !isEnabled;

  // if (isEnabled) {
  //   Monitor.println("ACTIVATING MOTORS");
  //   StartMotors(1000.00);
  // }
  // else {
  //   Monitor.println("DEACTIVATING MOTORS");
  //   StopMotors();
  // }
}

void InitMotors() {
  pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);
  analogWriteResolution(10);
  analogWrite(LEFT_MOTOR_PWM_PIN, 0);
  analogWrite(RIGHT_MOTOR_PWM_PIN, 0);
  pinMode(LEFT_MOTOR_HALL_PIN, INPUT);
  pinMode(RIGHT_MOTOR_HALL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_HALL_PIN), leftMotorISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_HALL_PIN), rightMotorISR, RISING);
}

void StartMotors(double rpm) {
  int outputValue = map(rpm, 0, 4300, 0, 1024);
  Monitor.print("Output value: ");
  Monitor.println(outputValue);
  Monitor.println("Starting motors");
  analogWrite(LEFT_MOTOR_PWM_PIN, outputValue);
  analogWrite(RIGHT_MOTOR_PWM_PIN, outputValue);
}
void StopMotors() {
  analogWrite(LEFT_MOTOR_PWM_PIN, 0);
  analogWrite(RIGHT_MOTOR_PWM_PIN, 0);
}

int GetRPM(Side motor) {
    volatile unsigned long *pulsePeriod   = (motor == LEFT) ? &leftPulsePeriod   : &rightPulsePeriod;
    volatile unsigned long *lastPulseTime = (motor == LEFT) ? &leftLastPulseTime : &rightLastPulseTime;

    noInterrupts();
    unsigned long period   = *pulsePeriod;
    unsigned long lastTime = *lastPulseTime;
    interrupts();

    // 0 if no pulse has arrived within the timeout window
    if (period == 0 || (micros() - lastTime) > 500000UL) return 0;

    Monitor.print("RPM reading:");
    Monitor.println((1000000UL / period));
    return (int)((1000000.0f / period) * 30.0f);
}
