#include "Sensors.h"
#include <Adafruit_AMG88xx.h>
#include <Arduino_RouterBridge.h>
#include <Adafruit_LIS2MDL.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <math.h>
#include <Wire.h>


// Addresses
#define GY521Address 0x68
#define SonarAddress 0x70
#define LSM303AGR_ACC_ADDRESS 0x19 // 0011001X
#define LSM303AGR_MAG_ADDRESS 0x1E //0011110b

// Pins
#define UART_RX D0
#define UART_TX D1
#define GPS_BAUD_RATE 9600
#define ANEMOMETER_PIN A0
#define WIND_VANE_RX 12
#define WIND_VANE_TX 13

const double MAG_OFFSETS[3] = { // use calibration script taken from Adafruit website
  (13.50 - 86.55) / 2.0, // x
  (77.85 - 37.45) / 2.0, // y
  (78.60 - 45.55) / 2.0  // z
};

struct {
  int scl;
  int sda;
} Pins::GY521 = {B10, B11};


// init sensor objects
Adafruit_LIS2MDL Mag = Adafruit_LIS2MDL(12345);
TinyGPSPlus gps;                 // GPS
Adafruit_AMG88xx ThermalCamera;  // Thermal camera

// Declare wind-vane serial
SoftwareSerial windVaneSerial(WIND_VANE_RX, WIND_VANE_TX);

// Declare wind-vane serial
SoftwareSerial windVaneSerial(WIND_VANE_RX, WIND_VANE_TX);


void InitSensors() {
  Wire.begin(); // begin I2c communication
  // InitGY521();
  InitSonar();
  InitMagnetometer();
  InitGPS();
  InitThermalCamera();
}

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
  ret.AcX = Wire.read()<<8|Wire.read();    
  ret.AcY = Wire.read()<<8|Wire.read();  
  ret.AcZ = Wire.read()<<8|Wire.read(); 
  ret.Tmp = Wire.read()<<8|Wire.read(); 
  ret.GyX = Wire.read()<<8|Wire.read();  
  ret.GyY = Wire.read()<<8|Wire.read();  
  ret.GyZ = Wire.read()<<8|Wire.read();

  return ret;
}

GY521Orientation GetGY521Orientation(const GY521Data& data) {
  GY521Orientation ret;
  double x = data.AcX;
  double y = data.AcY;
  double z = data.AcZ;
  
  ret.pitch = atan( x / sqrt((y * y) + (z * z)));
  ret.roll = atan(y / sqrt((x * x) + (z * z)));
  ret.pitch = ret.pitch * (180.0 / 3.14);
  ret.roll = ret.roll * (180.0 / 3.14) ;  

  return ret;
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
  if (!Mag.begin()) {
    Monitor.println("Something went wrong initializing the magnetometer");
  }
  Mag.begin();
}


LSM303AGRData GetMagnetometerData() {
  sensors_event_t event;
  LSM303AGRData ret;
  int32_t data[3];

  Mag.getEvent(&event);

  ret.MagX = event.magnetic.x;
  ret.MagY = event.magnetic.y;
  ret.MagZ = event.magnetic.z;

  return ret;
}

float GetHeading() {
  float heading;

  LSM303AGRData data = GetMagnetometerData(); // expressed in mG
  heading = atan2(data.MagY - MAG_OFFSETS[1], data.MagX - MAG_OFFSETS[0]) * (180.0 / 3.141592);

  if (heading < 0) {
    return heading + 360;
  }
  return heading;
}

void InitGPS() {
  Serial.begin(GPS_BAUD_RATE); // GPS is connected to UART TX and RX pins
}

GPSData GetGPSData() {
  // Monitor.println("getting GPS data");
  GPSData ret;
  while (Serial.available()) { // while there is input at UART pins
    char x = (char)Serial.read();
    // Monitor.print(x);
    gps.encode(x); // TinyGPSPlus handles the NMEA encoding
  }
  // Monitor.println("");

  ret.lat = gps.location.lat();
  ret.lon = gps.location.lng();
  ret.hdop = gps.hdop.hdop();
  return ret;
}

void InitThermalCamera() {
  if (!ThermalCamera.begin()) {
    Monitor.println("ERROR ENCOUNTERED WHILE STARTING THERMAL CAMERA!!");
  } 
}

void GetThermalCameraData(float* pixels) {
  ThermalCamera.readPixels(pixels); // TODO: don't use C-style arrays to prevent out-of-bounds indexing
}

void InitAnemometer() {

}

int GetAnemometerData() {
  return analogRead(ANEMOMETER_PIN);
}

float GetWindSpeed() {
  int idle_noise = 180; // raw ADC value detected when anemometer isn't spinning
  // TODO: finish this and find out how to limit ADC noise
}

void InitWindVane() {

}

float GetWindDirection() {

}

bool IsPersonDetected() { // TODO: finish this function!
  const int DETECTION_CUTOFF = 70; 
  return GetSonarData() < DETECTION_CUTOFF;
}
