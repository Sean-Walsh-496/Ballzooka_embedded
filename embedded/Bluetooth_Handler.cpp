#include "Bluetooth_Handler.h"
#include <Arduino_RouterBridge.h>
#include "Sensors.h"

#define SENSOR_SERVICE_UUID "ba10f731-f94d-45f8-8ccd-89e393b418f4"
#define HEADING_CHARACTERISTIC_UUID "ba10f732-f94d-45f8-8ccd-89e393b418f4"
#define LAT_CHARACTERISTIC_UUID "ba10f733-f94d-45f8-8ccd-89e393b418f4"
#define LON_CHARACTERISTIC_UUID "ba10f736-f94d-45f8-8ccd-89e393b418f4"
#define BATTERY_CHARACTERISTIC_UUID "ba10f734-f94d-45f8-8ccd-89e393b418f4"
#define RPM_CHARACTERISTIC_UUID "ba10f735-f94d-45f8-8ccd-89e393b418f4"

BLEService sensorService(SENSOR_SERVICE_UUID); //                    two-byte integers
BLEIntCharacteristic HeadingCharacteristic(HEADING_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic LonCharacteristic(LON_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic LatCharacteristic(LAT_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic BatteryCharacteristic(BATTERY_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic RPMCharacteristic(RPM_CHARACTERISTIC_UUID, BLERead);

// FUNCTION DEFINITIONS ========================================================
bool InitBluetooth() {
  if (BLE.begin()) {
    Monitor.println("Successfully initialized Bluetooth service!");

    // Name that appears when in "discovery" mode
    BLE.setLocalName("Ballzooka");

    // advertises the service
    BLE.setAdvertisedService(sensorService);
    
    // add the characteristics
    sensorService.addCharacteristic(HeadingCharacteristic);
    sensorService.addCharacteristic(LonCharacteristic);
    sensorService.addCharacteristic(LatCharacteristic);
    sensorService.addCharacteristic(BatteryCharacteristic);
    sensorService.addCharacteristic(RPMCharacteristic);

    // init the service
    BLE.addService(sensorService);

    // write initial values
    HeadingCharacteristic.writeValue(0);
    LonCharacteristic.writeValue(0);
    LatCharacteristic.writeValue(0);
    BatteryCharacteristic.writeValue(0);
    RPMCharacteristic.writeValue(0);

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

/**
 * @brief Updates sensor service's characteristics to fit currently read sensor
 * values
 */
void UpdateSensorService() {
  // heading update
  HeadingCharacteristic.setValue(GetHeading());

  // position update
  GPSData pos = GetGPSData();
  LonCharacteristic.setValue((int)(pos.lon ));
  LatCharacteristic.setValue((int)(pos.lat * 1000));

}