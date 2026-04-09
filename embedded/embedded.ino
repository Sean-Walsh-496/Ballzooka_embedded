#include <Arduino_RouterBridge.h> // Micro-controller cannot access USB-C serial
#include <Wire.h>

// User-defined libraries
#include "Bluetooth_Handler.h"
#include "Driving_Motor.h"
#include "Sensors.h"
#include "State_Machine.h"
#include "Stepper_Motor.h"
#include "Bluetooth_Handler.h"


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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // set state machine to default
  currentState = EnterConnect(true);

  // Init sensors
  Wire.begin(); // begin I2c communication
  // Serial.begin(115200);
  // InitGY521();
  InitSonar();
  InitMagnetometer();
  InitGPS();
  InitThermalCamera();

  // init flywheel motors
  InitMotors();

  // Offers some functions to the MPU
  // Bridge.begin(); // initialize software bridge between MCU and MPU


  Monitor.begin(115200); // init console logging
  delay(1000); // wait for Monitor to initiaize (not really necessary just being safe)
  Monitor.println("Ballzooka powered on.");
  
  pinMode(LED3_B, OUTPUT);
  digitalWrite(LED3_B, LOW);

}

void loop() {
  int dist = GetSonarData();
  Monitor.print("DISTANCE FROM SONAR: ");
  Monitor.println(dist);

  // verify Bluetooth is still connected
  if (! HasBluetoothConnection()) { // TODO: maybe check this less frequently or in a separate thread
    currentState = EnterConnect(false);
  }

  // update sensor service data
  UpdateSensorService();

  ExecuteCommands();

  // state machine
  switch(currentState) {
    case CONNECT:
      currentState = HandleConnect();
      break;
    case IDLE_SAFE:
      currentState = HandleIdleSafe();
      break;
    case IDLE_DANGER:
      break;
    case REPOSITION:
      break;
    case STANDBY:
      break;
    case LAUNCH:
      break;
  }

  // log and change LED
  if (CONSOLE_LOGGING) {
    PrintStatus();
  }

}
