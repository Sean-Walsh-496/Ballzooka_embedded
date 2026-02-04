#ifndef BLUETOOTH_HANDLER
#define BLUETOOTH_HANDLER

#include <ArduinoBLE.h>

extern const BLEService heartBeatService;
extern const BLEByteCharacteristic stateCharacteristic;

// FUNCTIONS ===================================================================
bool InitBluetooth();
bool HasBluetoothConnection();

#endif