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
BallzookaData ballzooka_data; // struct which contains relevant data about 
                              // cannon state

// FUNCTIONS ===================================================================

/**
 * @brief Debugging function that logs the current state of the cannon after 
 * each iteration through the state machine loop
 */
void PrintStatus() {
  LOG("Current state is: ");
  LOG(stateNames[ballzooka_data.current_state]);
  LOG("\r\n");
}

void setup() { // runs once on Arduino startup
  ballzooka_data = InitBallzookaData(); // set data struct to default values
  InitSensors(); // initialize all sensors

  // init flywheel motors
  InitMotors();

  Monitor.begin(115200); // init console logging with 115,200 BAUD
  delay(1000); // wait for Monitor to initiaize (not really necessary just being safe)
  Monitor.println("Ballzooka powered on.");
  
  pinMode(LED3_B, OUTPUT); // flip LED value to visually show program is running

}

// Loop that continuously executes during Arduino's runtime
void loop() {
  // verify Bluetooth is still connected
  if (! HasBluetoothConnection()) { // TODO: maybe check this less frequently or in a separate thread
    EnterConnect(ballzooka_data, false); // enter connect state
  }

  // check if person has been detected
  if (IsPersonDetected()) {
    LOG("!!!!! PERSON DETECTED !!!!!\r\n");
    ballzooka_data.person_detected = true; // update state struct

    // since this characteristic has BLE notifications enabled, this line will 
    // push a notification to the app as well.
    PersonDetectedCharacteristic.setValue(true); 
    EnterIdleSafe(ballzooka_data);

  }
  else {
    ballzooka_data.person_detected = false;
    PersonDetectedCharacteristic.setValue(false);

  }

  // update BLE sensor service that Anrdoid app is receiving from
  UpdateSensorService(ballzooka_data);

  // Check if Android app has transmitted any commands to the Arduino
  ReceiveCommands(ballzooka_data);

  // state machine handler. See State_Machine.cpp for function logic
  switch(ballzooka_data.current_state) {
    case CONNECT:
      HandleConnect(ballzooka_data);
      break;
    case IDLE_SAFE:
      HandleIdleSafe(ballzooka_data);
      break;
    case IDLE_DANGER:
      HandleIdleDanger(ballzooka_data);
      break;
    case REPOSITION:
      HandleReposition(ballzooka_data);
      break;

    // Since we never built a loading mechanism, the logic for these states was
    // never implemented
    case STANDBY:
      break;
    case LAUNCH:
      break;
  }

  PrintStatus();
}
