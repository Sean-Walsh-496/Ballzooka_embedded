#include <Arduino_RouterBridge.h> // Micro-controller cannot access USB-C serial

// User-defined libraries
#include "State_Machine.h"

// PROGRAM SETTINGS
#define CONSOLE_LOGGING true


// GLOBALS =====================================================================
State currentState;


// FUNCTIONS ===================================================================
void PrintStatus() {
  Monitor.print("Current state is: ");
  Monitor.println(stateNames[currentState]);
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
