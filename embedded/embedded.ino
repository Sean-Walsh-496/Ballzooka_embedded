#include <Arduino_RouterBridge.h> // Micro-controller cannot access USB-C serial
#include <Wire.h>

// User-defined libraries
#include "Bluetooth_Handler.h"
#include "Driving_Motor.h"
#include "Helper.h"
#include "Sensors.h"
#include "State_Machine.h"
#include "Stepper_Motor.h"
#include "Bluetooth_Handler.h"

// GLOBALS =====================================================================
BallzookaData ballzooka_data;

// FUNCTIONS ===================================================================
void PrintStatus() {
  LOG("Current state is: ");
  LOG(stateNames[ballzooka_data.current_state]);
  LOG("\r\n");
}

void setup() {
  ballzooka_data = InitBallzookaData();
  InitSensors();

  // init flywheel motors
  InitMotors();

  Monitor.begin(115200); // init console logging
  delay(1000); // wait for Monitor to initiaize (not really necessary just being safe)
  Monitor.println("Ballzooka powered on.");
  
  pinMode(LED3_B, OUTPUT);
  digitalWrite(LED3_B, LOW);

}

void loop() {
  int dist = GetSonarData();
  LOG("SONAR DISTANCE: "); 
  LOG(dist); 
  LOG("\r\n");

  // verify Bluetooth is still connected
  if (! HasBluetoothConnection()) { // TODO: maybe check this less frequently or in a separate thread
    EnterConnect(ballzooka_data, false);
  }

  // update sensor service data
  UpdateSensorService();

  ReceiveCommands(ballzooka_data);

  // state machine
  switch(ballzooka_data.current_state) {
    case CONNECT:
      HandleConnect(ballzooka_data);
      break;
    case IDLE_SAFE:
      HandleIdleSafe(ballzooka_data);
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

  PrintStatus();
}
