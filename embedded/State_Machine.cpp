#include "State_Machine.h"
#include "Bluetooth_Handler.h"
#include "Helper.h"

const char* stateNames[NUM_STATES] = {"CONNECT", "IDLE_SAFE", "IDLE_DANGER", "REPOSITION", "STANDBY", "LAUNCH"};

// Function Definitions ========================================================
BallzookaData InitBallzookaData() {
  BallzookaData data;
  EnterConnect(data, true);
  data.has_received_command = false;
  data.target_RPM = 0;
  data.target_yaw = 0;

  return data;
}


void EnterConnect(BallzookaData &data, bool firstTime) {
  if (firstTime) {
    InitBluetooth();
  }
  else {
    AdvertiseBluetooth();
  }
  data.current_state = CONNECT;
}

void HandleConnect(BallzookaData &data) {
  if (HasBluetoothConnection()) {
    data.current_state = IDLE_SAFE;
  }
}

void HandleIdleSafe(BallzookaData &data) {
  if (data.has_received_command) {
    data.current_state = REPOSITION;
  }
}

void HandleReposition(BallzookaData &data) {


}