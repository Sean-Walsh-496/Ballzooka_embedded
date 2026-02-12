#include "Sensors.h"
#include <Wire.h>
#include <Arduino_RouterBridge.h>


// GY-521 code
#define GY521Address 0x68
#define SonarAddress 0x70
#define LSM303AGR_ACC_ADDRESS 0x19 // 0011001X
#define LSM303AGR_MAG_ADDRESS 0x1E //0011110b

struct {
  int scl;
  int sda;
} Pins::GY521 = {B10, B11};

void InitGY521() {

  // wake GY521
  Wire.beginTransmission(GY521Address);
  Wire.write(0x6B); // set address to GY521 power management address
  Wire.write(0x00); // send wake up command to power address

  Wire.endTransmission(true);
}

GY521Data GetGY521Data() {
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

GY521Orientation GetGY521Orientation(const GY521Data& data) {
  GY521Orientation ret;
  ret.pitch = atan( data.AcX / sqrt((data.AcY * data.AcY) + (data.AcZ * data.AcZ)));
  ret.roll = atan(data.AcY / sqrt((data.AcX * data.AcX) + (data.AcZ * data.AcZ)));
  ret.pitch = ret.pitch * (180.0 / 3.14);
  ret.roll = ret.roll * (180.0 / 3.14) ;  
}

// Sonar code
void InitSonar() {
  
}

int GetSonarData() {

  Wire.beginTransmission(SonarAddress);
  Wire.write(0x51); // command a range reading
  Wire.endTransmission(false);

  delay(150); // wait to save the reading

  Wire.requestFrom(SonarAddress, 2, true); // read the distance

  return Wire.read()<<8|Wire.read(); // read high and low byte and append them

}

void InitMagnetometer() {
  Wire.beginTransmission(LSM303AGR_MAG_ADDRESS);
  // Wire.write()
}


LSM303AGRData GetMagnetometerData() {
  LSM303AGRData ret;

  const int ADDRESSES[] = {0x69, 0x68, 0x6B, 0x6A, 0x6D, 0x6C};
  for (int i = 0; i < 6; i+=2) { // TODO: make this suck less
    int test;
    Wire.beginTransmission(LSM303AGR_MAG_ADDRESS);
    Wire.write(ADDRESSES[i]);
    Wire.endTransmission(false);
    Wire.requestFrom(LSM303AGR_MAG_ADDRESS, 1, true);

    test = Wire.read();
    test<<8;

    Wire.beginTransmission(LSM303AGR_MAG_ADDRESS);
    Wire.write(ADDRESSES[i + 1]);
    Wire.endTransmission(false);
    Wire.requestFrom(LSM303AGR_MAG_ADDRESS, 1, true);

    test |= Wire.read();

    Monitor.println(test);
  }

  return ret;
}
