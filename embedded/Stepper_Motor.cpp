#include "Stepper_Motor.h"
#include "math.h"

/**
 * @brief Given the current yaw (yaw being defined in README.md and Stepper_Motor.h) and
 * target yaw, compute the rotation command needed to rotate the cannon to the desired 
 * location.
 */
RotationCommand GetRotationCommand(float current_yaw, float new_yaw) {
  RotationCommand final_command;
  
  Direction dir = (current_yaw > new_yaw) ? Direction::CCW : Direction::CW;
  float diff = fabs(current_yaw - new_yaw);
  

  if (diff > 180) { // longest possible path should sweep at most 180 degrees
    diff = fabs(360 - diff);
    dir = (dir == Direction::CW) ? Direction::CCW : Direction::CW;
  }

  final_command.degrees = diff;
  final_command.dir = dir;

  return final_command;
}

void RotateCW() {

}

void RotateCCW() {

}

void StopRotate() {

}

void RotateCannon(RotationCommand command) {
  const float MARGIN_OF_ERROR = 5; // in degrees. Pretty rough but maybe good enough
  // get current heading
  float cur_heading;

  // compute final heading
  float final_heading = cur_heading + command.degrees;

  // start rotation 
  switch(command.dir) {
    case Direction::CCW:
      RotateCCW();
      break;
    case Direction::CW:
      RotateCW();
      break;
    default:
      break;
  }


  // rotate until within range. TODO: stop this from overshooting or busy waiting
  while (fabs(cur_heading - final_heading) > MARGIN_OF_ERROR) {}

  // stop rotation
  StopRotate();
}

  // setup
  // pinMode(PULSE_PIN, OUTPUT);
  // pinMode(HI_PIN, OUTPUT);

  // digitalWrite(PULSE_PIN, LOW);
  // digitalWrite(HI_PIN, HIGH);


  // loop
  // digitalWrite(PULSE_PIN, LOW);
  // delay(10);
  // digitalWrite(PULSE_PIN, HIGH);
  // delay(10);

