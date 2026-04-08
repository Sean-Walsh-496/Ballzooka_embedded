// #ifndef DRIVING_MOTOR_H
// #define DRIVING_MOTOR_H

enum Side {
  LEFT,
  RIGHT
};

void RespondToButton();
void InitMotors();
void StartMotors(double rpm);
void StopMotors();

int GetRPM(Side motor);



// #endif