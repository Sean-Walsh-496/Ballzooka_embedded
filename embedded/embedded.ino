#include <Arduino_RouterBridge.h> // Micro-controller cannot access USB-C serial
#include <Wire.h>

// User-defined libraries
#include "Bluetooth_Handler.h"
#include "Sensors.h"
#include "State_Machine.h"


// PROGRAM SETTINGS
#define CONSOLE_LOGGING false

#define PULSE_PIN D7
#define HI_PIN D8


// GLOBALS =====================================================================
State currentState;


// FUNCTIONS ===================================================================
void PrintStatus() {
  Monitor.print("Current state is: ");
  Monitor.println(stateNames[currentState]);
}


void setup() {
  // set state machine to default
  currentState = EnterConnect();

  // Init additional functionality
  // InitBluetooth();
  Wire.begin(); // begin I2c communication
  // InitGY521();
  // InitSonar();


  Monitor.begin();
  delay(1000);
  Monitor.println("Ballzooka powered on.");


  pinMode(PULSE_PIN, OUTPUT);
  pinMode(HI_PIN, OUTPUT);

  digitalWrite(PULSE_PIN, LOW);
  digitalWrite(HI_PIN, HIGH);

}

void loop() {



  digitalWrite(PULSE_PIN, LOW);
  delay(10);
  digitalWrite(PULSE_PIN, HIGH);
  delay(10);


  // GetMagnetometerData();
  // delay(1000);

  // // state machine behavior
  // switch(currentState) {
  //   case CONNECT:
  //     currentState = HandleConnect();
  //     break;
  //   case IDLE_SAFE:
  //     currentState = HandleIdleSafe();
  //     break;
  // }

  // // log and change LED
  // if (CONSOLE_LOGGING) {
  //   PrintStatus();
  // }

}
