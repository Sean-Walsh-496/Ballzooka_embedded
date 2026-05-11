#include "Sensors.h"
#include "State_Machine.h"
#include "Stepper_Motor.h"
#include "Bluetooth_Handler.h"
#include "Helper.h"

const char* stateNames[NUM_STATES] = {"CONNECT", "IDLE_SAFE", "IDLE_DANGER", "REPOSITION", "STANDBY", "LAUNCH"};

// Function Definitions ========================================================


BallzookaData InitBallzookaData() {
  BallzookaData data;
  EnterConnect(data, true);
  data.has_received_command = false;
  data.person_detected = false;
  data.target_RPM = 0;
  data.target_yaw = 0;

  return data;
}

/**
 * Puts cannon-state struct into connect state and handles any one-time tasks 
 * that are required when entering the state.
 *
 * @param data Cannon-state struct
 * @param firstTime whether or not this is the first time the cannon is entering
 * the connect state
 */
void EnterConnect(BallzookaData &data, bool firstTime) {
  // if you haven't entered this state before, initialize the BLE service
  if (firstTime) {
    InitBluetooth();
  }
  else {
    AdvertiseBluetooth();
  }
  data.current_state = CONNECT;
}

/**
 * All the connect state will do is wait for a Bluetooth connection to be 
 * established
 *
 * @param data Cannon-state struct
 */
void HandleConnect(BallzookaData &data) {
  if (HasBluetoothConnection()) {
    data.current_state = IDLE_SAFE;
  }
}


/**
 * Enter the idle safe state (no running motors)
 *
 * @param data Cannon-state struct
 */
void EnterIdleSafe(BallzookaData &data) {
  // TODO: this function is unfinished. Currently the logic for disabling motors 
  // and servos is done by the Bluetooth module, but this isn't a reasonable 
  // place for the logic to reside.
  data.current_state = IDLE_SAFE;

  // disable motors

  // disable servos

  // send message to user
}

/**
 * The Idle state, by definition, doesn't actively do anything, simply waiting 
 * for a command to be sent by the Android app.
 *
 * @param data Cannon-state struct
 */
void HandleIdleSafe(BallzookaData &data) {
  if (data.has_received_command) {
    data.current_state = REPOSITION;
  }
}

void HandleReposition(BallzookaData &data) {
  // First determine how much the cannon has to rotate (using current heading
  // and desired heading), then rotate that amount
  if (RotateCannon(GetRotationCommand(GetHeading(), data.target_yaw))) {
    return;
  }
  else { // Once reposition is complete, stand by with motors running
    data.current_state = IDLE_DANGER;
  }
}

void HandleIdleDanger(BallzookaData &data) {

}