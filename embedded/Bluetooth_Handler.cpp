#include "Bluetooth_Handler.h"
#include <Arduino_RouterBridge.h>

#define SENSOR_SERVICE_UUID "ba10f731-f94d-45f8-8ccd-89e393b418f4"
#define SENSOR_TEST_CHARACTERISTIC_UUID "ba10f732-f94d-45f8-8ccd-89e393b418f4"

BLEService sensorService(SENSOR_SERVICE_UUID);
BLEByteCharacteristic sensorStateCharacteristic(SENSOR_TEST_CHARACTERISTIC_UUID, BLERead);


// FUNCTION DEFINITIONS ========================================================
bool InitBluetooth() {
  if (BLE.begin()) {
    Monitor.println("Successfully initialized Bluetooth service!");

    // Name that appears when in "discovery" mode
    BLE.setLocalName("Ballzooka");

    // advertises the service
    BLE.setAdvertisedService(sensorService);
    
    sensorService.addCharacteristic(sensorStateCharacteristic);

    BLE.addService(sensorService);

    sensorStateCharacteristic.writeValue(10);

    AdvertiseBluetooth();
    return true;
  }
  Monitor.println("Error initializing Bluetooth service!");
  return false;
}

void AdvertiseBluetooth() {
  BLE.advertise();
}

bool HasBluetoothConnection() {
  return BLE.connected();
}