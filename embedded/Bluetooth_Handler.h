#ifndef BLUETOOTH_HANDLER
#define BLUETOOTH_HANDLER

#include <ArduinoBLE.h>
#include "State_Machine.h"

extern BLEService sensorService;
extern BLEDoubleCharacteristic  HeadingCharacteristic;
extern BLEDoubleCharacteristic  LonCharacteristic;
extern BLEDoubleCharacteristic  LatCharacteristic;
extern BLEIntCharacteristic     BatteryCharacteristic;
extern BLEDoubleCharacteristic     RPMCharacteristic;
extern BLEBoolCharacteristic     PersonDetectedCharacteristic;

extern BLEDescriptor LatUserDescriptor;
extern BLEDescriptor LonUserDescriptor;
extern BLEDescriptor HeadingDescriptor;

// FUNCTIONS ===================================================================
bool InitBluetooth();
bool HasBluetoothConnection();
void AdvertiseBluetooth();
void UpdateSensorService(BallzookaData &data);
void ReceiveCommands(BallzookaData &data);

#endif