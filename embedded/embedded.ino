#include <Arduino_RouterBridge.h> // Micro-controller cannot access USB-C serial
#include <Wire.h>

// User-defined libraries
#include "Bluetooth_Handler.h"
#include "Sensors.h"
#include "State_Machine.h"


// PROGRAM SETTINGS
#define CONSOLE_LOGGING false


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
  InitSonar();


  Monitor.begin();
  delay(1000);
  Monitor.println("Ballzooka powered on.");

}

void loop() {



  Monitor.println("Getting sonar data");
  int data = GetSonarData();
  Monitor.println(data);
  delay(1000);

  // GY521Data inData = GetGY521Data();
  // GY521Orientation orient = GetGY521Orientation(inData);

  // Monitor.println("New Reading");
  // Monitor.println(orient.pitch);
  // Monitor.println(orient.roll);
  // Monitor.println(orient.yaw);
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
