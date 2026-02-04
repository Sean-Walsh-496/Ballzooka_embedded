#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// ENUMS =======================================================================
#define NUM_STATES 6
enum State {
  CONNECT,
  IDLE_SAFE,
  IDLE_DANGER,
  REPOSITION,
  STANDBY,
  LAUNCH
};


// GLOBALS =====================================================================
extern const char* stateNames[NUM_STATES];


// FUNCTIONS ===================================================================
State HandleConnect();
State HandleIdleSafe();


#endif