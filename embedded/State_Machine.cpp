#include "State_Machine.h"
#include "Bluetooth_Handler.h"
#include "Helper.h"

const char* stateNames[NUM_STATES] = {"CONNECT", "IDLE_SAFE", "IDLE_DANGER", "REPOSITION", "STANDBY", "LAUNCH"};

// Function Definitions ========================================================
BallzookaData InitBallzookaData() {
  BallzookaData data;
  data.current_state = EnterConnect(true);
  data.has_received_comamnd = false;
  data.target_RPM = 0;
  data.target_yaw = 0;

  return data;
}


State EnterConnect(bool firstTime) {
  if (firstTime) {
    InitBluetooth();
  }
  else {
    AdvertiseBluetooth();
  }
  return CONNECT;
}

State HandleConnect() {
  if (HasBluetoothConnection()) {
    return IDLE_SAFE;
  }
  return CONNECT;
}

State HandleIdleSafe() {
  if (HasValidTarget()) {
    return REPOSITION;
  }
  return IDLE_SAFE;
}

State HandleEnterReposition() {
  return REPOSITION;
}

State HandleReposition() {

}