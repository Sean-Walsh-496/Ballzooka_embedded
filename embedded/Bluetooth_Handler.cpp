#include "Bluetooth_Handler.h"
#include <Arduino_RouterBridge.h>
#include "Sensors.h"

#define SENSOR_SERVICE_UUID "ba10f731-f94d-45f8-8ccd-89e393b418f4"
#define HEADING_CHARACTERISTIC_UUID "ba10f732-f94d-45f8-8ccd-89e393b418f4"
#define LAT_CHARACTERISTIC_UUID "ba10f733-f94d-45f8-8ccd-89e393b418f4"
#define LON_CHARACTERISTIC_UUID "ba10f736-f94d-45f8-8ccd-89e393b418f4"
#define BATTERY_CHARACTERISTIC_UUID "ba10f734-f94d-45f8-8ccd-89e393b418f4"
#define RPM_CHARACTERISTIC_UUID "ba10f735-f94d-45f8-8ccd-89e393b418f4"
#define PERSON_DETECTED_CHARACTERISTIC_UUID "ba10f737-f94d-45f8-8ccd-89e393b418f4"

BLEService                sensorService(SENSOR_SERVICE_UUID); //              two-byte integers
BLEDoubleCharacteristic   HeadingCharacteristic(HEADING_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEDoubleCharacteristic   LonCharacteristic(LON_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEDoubleCharacteristic   LatCharacteristic(LAT_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEIntCharacteristic      BatteryCharacteristic(BATTERY_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic      RPMCharacteristic(RPM_CHARACTERISTIC_UUID, BLERead);
BLEBoolCharacteristic     PersonDetectedCharacteristic(PERSON_DETECTED_CHARACTERISTIC_UUID, BLERead | BLENotify);

BLEDescriptor HeadingClientDescriptor("2902", "1");
BLEDescriptor LatClientDescriptor("2902", "1");
BLEDescriptor LonClientDescriptor("2902", "1");
BLEDescriptor BatteryClientDescriptor("2902", "1");
BLEDescriptor RPMClientDescriptor("2902", "1");
BLEDescriptor PersonDetectedClientDescriptor("2902", "1");


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
    sensorService.addCharacteristic(PersonDetectedCharacteristic);

    // add descriptors to each characteristic
    HeadingCharacteristic.addDescriptor(HeadingClientDescriptor);
    LatCharacteristic.addDescriptor(LatClientDescriptor);
    LonCharacteristic.addDescriptor(LonClientDescriptor);
    BatteryCharacteristic.addDescriptor(BatteryClientDescriptor);
    RPMCharacteristic.addDescriptor(RPMClientDescriptor);
    PersonDetectedCharacteristic.addDescriptor(PersonDetectedClientDescriptor);


    // init the service
    BLE.addService(sensorService);

    // write initial values
    HeadingCharacteristic.writeValue(0.0);
    LonCharacteristic.writeValue(0.0);
    LatCharacteristic.writeValue(0.0);
    BatteryCharacteristic.writeValue(0);
    RPMCharacteristic.writeValue(0);
    PersonDetectedCharacteristic.writeValue(false);

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

  // get values
  float heading = GetHeading();
  GPSData pos = GetGPSData();


  // print values
  Monitor.println("HEADING: " + String(heading));
  Monitor.println("LAT: " + String(pos.lat));
  Monitor.println("LON: " + String(pos.lon));
  Monitor.println("\r\n\r\n");

  HeadingCharacteristic.setValue(GetHeading());

  // position update
  LonCharacteristic.setValue(pos.lon);
  LatCharacteristic.setValue(pos.lat);


  
  // verify device does not have people in front of it 
  if (IsPersonDetected()) {
    Monitor.println("PERSON DETECTED!!!!");
    PersonDetectedCharacteristic.setValue(true);
  }
  else {
    PersonDetectedCharacteristic.setValue(false);
  }

}