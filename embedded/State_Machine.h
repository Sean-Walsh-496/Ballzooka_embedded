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

struct BallzookaData {
  State current_state;
  bool has_received_comamnd;
  float target_yaw;
  float target_RPM;
};


// GLOBALS =====================================================================
extern const char* stateNames[NUM_STATES];


// FUNCTIONS ===================================================================
BallzookaData InitBallzookaData();

State EnterConnect(bool firstTime);
State HandleConnect();

State EnterIdleSafe();
State HandleIdleSafe();

State HandleEnterReposition();
State HandleReposition();

#endif