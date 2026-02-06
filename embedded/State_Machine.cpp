#include "State_Machine.h"
#include "Bluetooth_Handler.h"
#include "Helper.h"

const char* stateNames[NUM_STATES] = {"CONNECT", "IDLE_SAFE", "IDLE_DANGER", "REPOSITION", "STANDBY", "LAUNCH"};

// Function Definitions ========================================================
State EnterConnect() {
  InitBluetooth();
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