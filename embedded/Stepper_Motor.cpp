#include <Arduino_RouterBridge.h>

#include "Stepper_Motor.h"
#include "Sensors.h"
#include "math.h"

/**
 * @brief Given the current yaw (yaw being defined in README.md and Stepper_Motor.h) and
 * target yaw, compute the rotation command needed to rotate the cannon to the desired 
 * location.
 */
RotationCommand GetRotationCommand(float current_yaw, float new_yaw) {
  RotationCommand final_command;
  
  // I love ternary operations
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

/**
 * Given a rotation command, turns the cannon in that direction
 *
 * @param command indicates how much to turn and in what direction
 */
bool RotateCannon(RotationCommand command) {
  const float MARGIN_OF_ERROR = 5; // in degrees. Pretty rough but maybe good enough
  // get current heading
  float cur_heading = GetHeading();

  // compute final heading
  float final_heading = cur_heading + command.degrees;

  // if outside our margin of error
  if (fabs(cur_heading - final_heading) > MARGIN_OF_ERROR) {
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
    Monitor.println("Rotating");
    return true;
  }
  else {
    // stop rotation
    StopRotate();
    return false;
  }

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

