#ifndef BLUETOOTH_HANDLER
#define BLUETOOTH_HANDLER

#include <ArduinoBLE.h>

extern BLEService sensorService;
extern BLEIntCharacteristic HeadingCharacteristic;
extern BLEIntCharacteristic PositionCharacteristic;
extern BLEIntCharacteristic BatteryCharacteristic;
extern BLEIntCharacteristic RPMCharacteristic;

// FUNCTIONS ===================================================================
bool InitBluetooth();
bool HasBluetoothConnection();
void AdvertiseBluetooth();
void UpdateSensorService();
#endif