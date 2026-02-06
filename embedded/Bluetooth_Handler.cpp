#include "Bluetooth_Handler.h"
#include <Arduino_RouterBridge.h>

#define SERVICE_UUID "ba10f731-f94d-45f8-8ccd-89e393b418f4"
#define TEST_CHARACTERISTIC_UUID "ba10f732-f94d-45f8-8ccd-89e393b418f4"

const BLEService heartBeatService(SERVICE_UUID);
const BLEByteCharacteristic stateCharacteristic(TEST_CHARACTERISTIC_UUID, BLERead | BLEWrite);


// FUNCTION DEFINITIONS ========================================================
bool InitBluetooth() {
  if (BLE.begin()) {
    Monitor.println("Successfully initialized Bluetooth service!");
    BLE.setLocalName("Ballzooka");
    BLE.setAdvertisedService(heartBeatService);
    BLE.advertise();
    return true;
  }
  Monitor.println("Error initializing Bluetooth service!");
  return false;
}

bool HasBluetoothConnection() {
  // TODO: finish this function!
  return true;
}