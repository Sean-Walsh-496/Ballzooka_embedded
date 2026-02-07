#ifndef SENSORS_H
#define SENSORS_H

namespace Pins {
  struct GY521;  
}

struct GY521Data {
  int AcX; // angle
  int AcY;
  int AcZ;
  int Tmp; // temperature
  int GyX; // gyroscope
  int GyY;
  int GyZ;
};

struct GY521Orientation {
  double pitch;
  double roll;
  double yaw;
};

void InitGY521();
GY521Data getGY521Data();
GY521Orientation getGY521Orientation(const GY521Data& data);



#endif
