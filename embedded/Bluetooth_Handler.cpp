#include "Bluetooth_Handler.h"
#include <Arduino_RouterBridge.h>
#include "Sensors.h"

// service UUID 
#define SENSOR_SERVICE_UUID "ba10f731-f94d-45f8-8ccd-89e393b418f4"

// RAW SENSOR READING UUIDs
#define HEADING_CHARACTERISTIC_UUID "ba10f732-f94d-45f8-8ccd-89e393b418f4"
#define LAT_CHARACTERISTIC_UUID "ba10f733-f94d-45f8-8ccd-89e393b418f4"
#define LON_CHARACTERISTIC_UUID "ba10f736-f94d-45f8-8ccd-89e393b418f4"
#define BATTERY_CHARACTERISTIC_UUID "ba10f734-f94d-45f8-8ccd-89e393b418f4"
#define RPM_CHARACTERISTIC_UUID "ba10f735-f94d-45f8-8ccd-89e393b418f4"
#define PERSON_DETECTED_CHARACTERISTIC_UUID "ba10f737-f94d-45f8-8ccd-89e393b418f4"

// COMMAND UUIDs
#define COMMAND_FLYWHEEL_RPM "ba10f738-f94d-45f8-8ccd-89e393b418f4"
#define COMMAND_LOADER_ANGLE "ba10f739-f94d-45f8-8ccd-89e393b418f4"


BLEService                sensorService(SENSOR_SERVICE_UUID);

// RAW CHARACTERISTICS FROM SENSORS
BLEDoubleCharacteristic   HeadingCharacteristic(HEADING_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEDoubleCharacteristic   LonCharacteristic(LON_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEDoubleCharacteristic   LatCharacteristic(LAT_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEIntCharacteristic      BatteryCharacteristic(BATTERY_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic      RPMCharacteristic(RPM_CHARACTERISTIC_UUID, BLERead);
BLEBoolCharacteristic     PersonDetectedCharacteristic(PERSON_DETECTED_CHARACTERISTIC_UUID, BLERead | BLENotify);

BLEDescriptor NotifyDescriptor("2902", "1");

// COMMAND CHARACTERISTICS FROM APP
BLEIntCharacteristic CommandFlywheelRPMCharacteristic(COMMAND_FLYWHEEL_RPM, BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic CommandLoaderAngleCharacteristic(COMMAND_LOADER_ANGLE, BLERead | BLEWrite | BLENotify);



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
    
    sensorService.addCharacteristic(CommandFlywheelRPMCharacteristic);
    sensorService.addCharacteristic(CommandLoaderAngleCharacteristic);
    

    // add descriptors to each characteristic
    HeadingCharacteristic.addDescriptor(NotifyDescriptor);
    LatCharacteristic.addDescriptor(NotifyDescriptor);
    LonCharacteristic.addDescriptor(NotifyDescriptor);
    BatteryCharacteristic.addDescriptor(NotifyDescriptor);
    RPMCharacteristic.addDescriptor(NotifyDescriptor);
    PersonDetectedCharacteristic.addDescriptor(NotifyDescriptor);
    CommandFlywheelRPMCharacteristic.addDescriptor(NotifyDescriptor);
    CommandLoaderAngleCharacteristic.addDescriptor(NotifyDescriptor);


    // init the service
    BLE.addService(sensorService);

    // write initial values
    HeadingCharacteristic.writeValue(0.0);
    LonCharacteristic.writeValue(0.0);
    LatCharacteristic.writeValue(0.0);
    BatteryCharacteristic.writeValue(0);
    RPMCharacteristic.writeValue(0);
    PersonDetectedCharacteristic.writeValue(false);
    CommandFlywheelRPMCharacteristic.writeValue(0);
    CommandLoaderAngleCharacteristic.writeValue(0);

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

/**
 * @brief Checks if the Bluetooth app has sent any commands and executes 
 * whatever has been received
 */
void ExecuteCommands() {
  int val;
  if (CommandFlywheelRPMCharacteristic.written()) {
    val = CommandFlywheelRPMCharacteristic.value();
    Monitor.print("READING COMMAND FOR FLYWHEEL RPM: ");
    // Monitor.println(val);
  }

  if (CommandLoaderAngleCharacteristic.written()) {
    val = CommandLoaderAngleCharacteristic.value();
    Monitor.print("READING COMMAND FOR LOADER ANGLE: ");
    // Monitor.println(val);
  }
}
