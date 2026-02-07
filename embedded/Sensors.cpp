#include "Sensors.h"
#include <Wire.h>


// GY-521 code
#define GY521Address 0x68

struct {
  int scl;
  int sda;
} Pins::GY521 = {B10, B11};



void InitGY521() {
  Wire.begin(); // begin I2c communication

  // wake GY521
  Wire.beginTransmission(GY521Address);
  Wire.write(0x6B); // set address to GY521 power management address
  Wire.write(0x00); // send wake up command to power address

  Wire.endTransmission(true);
}

GY521Data getGY521Data() {
  GY521Data ret;
  Wire.beginTransmission(GY521Address);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(GY521Address, 14, true); // 14 bytes of data

  // populate struct
  ret.AcX=Wire.read()<<8|Wire.read();    
  ret.AcY=Wire.read()<<8|Wire.read();  
  ret.AcZ=Wire.read()<<8|Wire.read();  
  ret.GyX=Wire.read()<<8|Wire.read();  
  ret.GyY=Wire.read()<<8|Wire.read();  
  ret.GyZ=Wire.read()<<8|Wire.read();

  return ret;
}

GY521Orientation getGY521Orientation(const GY521Data& data) {
  GY521Orientation ret;
  ret.pitch = atan( data.AcX / sqrt((data.AcY * data.AcY) + (data.AcZ * data.AcZ)));
  ret.roll = atan(data.AcY / sqrt((data.AcX * data.AcX) + (data.AcZ * data.AcZ)));
  ret.pitch = ret.pitch * (180.0 / 3.14);
  ret.roll = ret.roll * (180.0 / 3.14) ;  
}