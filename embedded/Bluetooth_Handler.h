#ifndef BLUETOOTH_HANDLER
#define BLUETOOTH_HANDLER

#include <ArduinoBLE.h>

extern BLEService sensorService;
extern BLEByteCharacteristic HeadingCharacteristic;
extern BLEByteCharacteristic PositionCharacteristic;
extern BLEByteCharacteristic BatteryCharacteristic;
extern BLEByteCharacteristic RPMCharacteristic;

// FUNCTIONS ===================================================================
bool InitBluetooth();
bool HasBluetoothConnection();
void AdvertiseBluetooth();
void UpdateSensorService();
#endif