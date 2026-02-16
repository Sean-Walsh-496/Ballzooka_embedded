#ifndef BLUETOOTH_HANDLER
#define BLUETOOTH_HANDLER

#include <ArduinoBLE.h>

extern BLEService heartBeatService;
extern BLEByteCharacteristic stateCharacteristic;

// FUNCTIONS ===================================================================
bool InitBluetooth();
bool HasBluetoothConnection();
void AdvertiseBluetooth();
#endif