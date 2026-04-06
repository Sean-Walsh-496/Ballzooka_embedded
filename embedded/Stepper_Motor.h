#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H


// Yaw represents horizontal rotation of cannon, determining which direction it faces. It 
// is represented as a floating-point value. The number corresponds to degrees from north.
// The orientation is rotated N degrees, clock-wise from a line cutting from south to 
// north, facing north corresponding to 0 degrees.

enum Direction {
  CW = 0,
  CCW = 1,
};

struct RotationCommand {
  Direction dir;
  float degrees;
};


RotationCommand GetRotationCommand(float current_yaw, float new_yaw);
void RotateCannon(RotationCommand command);



#endif