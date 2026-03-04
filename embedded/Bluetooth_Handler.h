#ifndef BLUETOOTH_HANDLER
#define BLUETOOTH_HANDLER

#include <ArduinoBLE.h>

extern BLEService sensorService;
extern BLEDoubleCharacteristic  HeadingCharacteristic;
extern BLEDoubleCharacteristic  LonCharacteristic;
extern BLEDoubleCharacteristic  LatCharacteristic;
extern BLEIntCharacteristic     BatteryCharacteristic;
extern BLEIntCharacteristic     RPMCharacteristic;

extern BLEDescriptor LatUserDescriptor;
extern BLEDescriptor LonUserDescriptor;
extern BLEDescriptor HeadingDescriptor;

// FUNCTIONS ===================================================================
bool InitBluetooth();
bool HasBluetoothConnection();
void AdvertiseBluetooth();
void UpdateSensorService();
#endif