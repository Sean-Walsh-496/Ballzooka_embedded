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


struct LSM303AGRData {
  int MagX;
  int MagY;
  int MagZ;
};



void InitGY521();
GY521Data GetGY521Data();
GY521Orientation GetGY521Orientation(const GY521Data& data);


void InitSonar();
int GetSonarData();

void InitMagnetometer();
LSM303AGRData GetMagnetometerData();

#endif
