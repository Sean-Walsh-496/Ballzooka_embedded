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


// GLOBALS
BallzookaData ballzooka_data;

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

  ballzooka_data = InitBallzookaData();

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
  LOG("SONAR DISTANCE: "); 
  LOG(dist); 
  LOG("\r\n");

  // verify Bluetooth is still connected
  if (! HasBluetoothConnection()) { // TODO: maybe check this less frequently or in a separate thread
    currentState = EnterConnect(false);
  }

  // update sensor service data
  UpdateSensorService();

  ExecuteCommands();

  // state machine
  switch(ballzooka_data.current_state) {
    case CONNECT:
      ballzooka_data.current_state = HandleConnect();
      break;
    case IDLE_SAFE:
      ballzooka_data.current_state = HandleIdleSafe();
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
