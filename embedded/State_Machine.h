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
  bool has_received_command;
  bool person_detected;

  // These fields are for when the cannon receives a command from the App. These
  // are checked against to ensure that the cannon is maintaining the desired 
  // position and RPM.
  float target_yaw;
  float target_RPM;
};


// GLOBALS =====================================================================
extern const char* stateNames[NUM_STATES]; // Used when logging current state


// FUNCTIONS ===================================================================
BallzookaData InitBallzookaData();

void EnterConnect(BallzookaData &data, bool firstTime);
void HandleConnect(BallzookaData &data);

void EnterIdleSafe(BallzookaData &data);
void HandleIdleSafe(BallzookaData &data);

void HandleReposition(BallzookaData &data);

void HandleIdleDanger(BallzookaData &data);

#endif