#include <Arduino_RouterBridge.h> // Micro-controller cannot access USB-C serial

// PROGRAM SETTINGS
#define CONSOLE_LOGGING true

// ENUMS =======================================================================
enum State {
  CONNECT,
  IDLE_SAFE,
  IDLE_DANGER,
  REPOSITION,
  STANDBY,
  LAUNCH
};


// GLOBALS =====================================================================
State currentState;
const char* stateNames[] = {"CONNECT", "IDLE_SAFE", "IDLE_DANGER", "REPOSITION", "STANDBY", "LAUNCH"};


// FUNCTIONS ===================================================================
void PrintStatus() {
  Monitor.print("Current state is: ");
  Monitor.println(stateNames[currentState]);
}

bool HasBluetoothConnection() {
  // TODO: finish this function!
  return true;
}

bool HasValidTarget() {
  return false;
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


void setup() {
  currentState = CONNECT;

  Monitor.begin();
  delay(1000);
  Monitor.println("Ballzooka powered on.");
}

void loop() {
  // check for failures

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
