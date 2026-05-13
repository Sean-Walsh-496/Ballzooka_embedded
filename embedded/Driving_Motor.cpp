#include "Driving_Motor.h"
#include <Arduino_RouterBridge.h>

// Defining the pins used for controlling the motor.
#define LEFT_MOTOR_PWM_PIN D5
#define RIGHT_MOTOR_PWM_PIN D6
#define LEFT_MOTOR_HALL_PIN D7
#define RIGHT_MOTOR_HALL_PIN D8

volatile unsigned long leftPulsePeriod = 0;
volatile unsigned long rightPulsePeriod = 0;
volatile unsigned long leftLastPulseTime = 0;
volatile unsigned long rightLastPulseTime = 0;

// interrupt service routines for the two motors
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

// Not currently being used, but allows you to easily program a button on the 
// Android application that when pressed can enable or disable the motors
void RespondToButton() {
  static bool isEnabled = false; // static, so value persists across function calls
  isEnabled = !isEnabled; // each time function is called, 
                          // alternate between enabling and disabling motors

  if (isEnabled) {
    Monitor.println("ACTIVATING MOTORS");
    StartMotors(1500);
  }
  else {
    Monitor.println("DEACTIVATING MOTORS");
    StopMotors();
  }
}

void InitMotors() {
    // Configuring PWN (pulse-width modulation) pins as outputs, which directly 
    // drive the motors
    pinMode(LEFT_MOTOR_PWM_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_PWM_PIN, OUTPUT);

    // PWM outputs default to 8-bit resolution, here we change it to 10 bits
    // increasing the number of discrete voltages that can be produced
    analogWriteResolution(10);
    analogWrite(LEFT_MOTOR_PWM_PIN, 0);
    analogWrite(RIGHT_MOTOR_PWM_PIN, 0);

    // configuring hall-effect pins as inputs (these track rotation of motors)
    pinMode(LEFT_MOTOR_HALL_PIN, INPUT);
    pinMode(RIGHT_MOTOR_HALL_PIN, INPUT);

    // Usefule for maintaining constant speed. Non interrupt approach didn't 
    // seem to work as well
    attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_HALL_PIN), leftMotorISR, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_HALL_PIN), rightMotorISR, RISING);
}

void StartMotors(double rpm) {
    // flip LED on to indicate motors are running
    digitalWrite(LED3_R, LOW);

    // Max RPM of 5000, and since we have a 10-bit resolution, we have to map 
    // the range of 0 - 5000 to some integer between 0 and 1024
    int outputValue = map(rpm, 0, 5000, 0, 1024);

    // debugging statements
    Monitor.print("Output value: ");
    Monitor.println(outputValue);
    Monitor.println("Starting motors");

    // write to PWM pins to drive motors
    analogWrite(LEFT_MOTOR_PWM_PIN, outputValue);
    analogWrite(RIGHT_MOTOR_PWM_PIN, outputValue);
}


void StopMotors() {
    // flip LED off and write 0 to PWM pins
    digitalWrite(LED3_R, HIGH);
    analogWrite(LEFT_MOTOR_PWM_PIN, 0);
    analogWrite(RIGHT_MOTOR_PWM_PIN, 0);
}

int GetRPM(Side motor) {
    volatile unsigned long *pulsePeriod   = (motor == LEFT) ? &leftPulsePeriod   : &rightPulsePeriod;
    volatile unsigned long *lastPulseTime = (motor == LEFT) ? &leftLastPulseTime : &rightLastPulseTime;

    // disable interrupts in atomic code piece
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


