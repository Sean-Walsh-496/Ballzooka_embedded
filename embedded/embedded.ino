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
  currentState = EnterConnect(true);

  // Init additional functionality
  // Wire.begin(); // begin I2c communication
  InitGY521();
  // InitSonar();
  // InitMagnetometer();
  InitGPS();


  Monitor.begin();
  delay(1000);
  Monitor.println("Ballzooka powered on.");

}

void loop() {
  GPSData ret = GetGPSData();
  Monitor.println(ret.lon);
  Monitor.println(ret.lat);

  delay(1000);

  // verify Bluetooth is still connected
  if (! HasBluetoothConnection()) { // TODO: maybe check this less frequently or in a separate thread
    currentState = EnterConnect(false);
  }

  // update sensor service data
  UpdateSensorService();

  // state machine
  switch(currentState) {
    case CONNECT:
      currentState = HandleConnect();
      break;
    case IDLE_SAFE:
      currentState = HandleIdleSafe();
      break;
  }

  // log and change LED
  if (CONSOLE_LOGGING) {
    PrintStatus();
  }

}
