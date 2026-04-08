#include "Bluetooth_Handler.h"
#include <Arduino_RouterBridge.h>
#include "Driving_Motor.h"
#include "Sensors.h"

#define PRINTING_SENSOR_DATA true

// service UUID
#define SENSOR_SERVICE_UUID "ba10f731-f94d-45f8-8ccd-89e393b418f4"

// RAW SENSOR READING UUIDs
#define HEADING_CHARACTERISTIC_UUID "ba10f732-f94d-45f8-8ccd-89e393b418f4"
#define LAT_CHARACTERISTIC_UUID "ba10f733-f94d-45f8-8ccd-89e393b418f4"
#define LON_CHARACTERISTIC_UUID "ba10f736-f94d-45f8-8ccd-89e393b418f4"
#define BATTERY_CHARACTERISTIC_UUID "ba10f734-f94d-45f8-8ccd-89e393b418f4"
#define LEFT_FLYWHEEL_RPM_CHARACTERISTIC_UUID "ba10f735-f94d-45f8-8ccd-89e393b418f4"
#define RIGHT_FLYWHEEL_RPM_CHARACTERISTIC_UUID "ba10f73a-f94d-45f8-8ccd-89e393b418f4"
#define PERSON_DETECTED_CHARACTERISTIC_UUID "ba10f737-f94d-45f8-8ccd-89e393b418f4"
#define PITCH_CHARACTERISTIC_UUID "ba10f73b-f94d-45f8-8ccd-89e393b418f4"

// COMMAND UUIDs
#define COMMAND_FLYWHEEL_RPM "ba10f738-f94d-45f8-8ccd-89e393b418f4"
#define COMMAND_YAW "ba10f739-f94d-45f8-8ccd-89e393b418f4"


BLEService                sensorService(SENSOR_SERVICE_UUID);

// RAW CHARACTERISTICS FROM SENSORS
BLEDoubleCharacteristic   HeadingCharacteristic(HEADING_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEDoubleCharacteristic   LonCharacteristic(LON_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEDoubleCharacteristic   LatCharacteristic(LAT_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEIntCharacteristic      BatteryCharacteristic(BATTERY_CHARACTERISTIC_UUID, BLERead);
BLEIntCharacteristic      LeftFlywheelRPMCharacteristic(LEFT_FLYWHEEL_RPM_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEIntCharacteristic      RightFlywheelRPMCharacteristic(RIGHT_FLYWHEEL_RPM_CHARACTERISTIC_UUID, BLERead | BLENotify);
BLEIntCharacteristic      PitchRPMCharacteristic(PITCH_CHARACTERISTIC_UUID, BLERead);
BLEBoolCharacteristic     PersonDetectedCharacteristic(PERSON_DETECTED_CHARACTERISTIC_UUID, BLERead | BLENotify);

BLEDescriptor NotifyDescriptor("2902", "1");

// COMMAND CHARACTERISTICS FROM APP
BLEDoubleCharacteristic CommandFlywheelRPMCharacteristic(COMMAND_FLYWHEEL_RPM, BLERead | BLEWrite | BLENotify);
BLEDoubleCharacteristic CommandYawCharacteristic(COMMAND_YAW, BLERead | BLEWrite | BLENotify);

// saved state
struct SavedData {
  float val;
  float threshold;
};

struct {
  SavedData heading;
  SavedData lon;
  SavedData lat;
  SavedData battery;
  SavedData leftFlywheelRPM;
  SavedData rightFlywheelRPM;
  bool personDetected;

} SavedState;

// FUNCTION DEFINITIONS ========================================================

void InitSavedState() {
  SavedState.heading;
}

/**
 * @brief Accomplishes three things:
 * 1) initialize and activate Bluetooth service
 * 2) initialize all Bluetooth characteristics
 * 3) Begin advertisting Bluetooth service to outside devices
 */
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
    sensorService.addCharacteristic(LeftFlywheelRPMCharacteristic);
    sensorService.addCharacteristic(RightFlywheelRPMCharacteristic);
    sensorService.addCharacteristic(PersonDetectedCharacteristic);
    sensorService.addCharacteristic(PitchRPMCharacteristic);

    sensorService.addCharacteristic(CommandFlywheelRPMCharacteristic);
    sensorService.addCharacteristic(CommandYawCharacteristic);


    // add descriptors to each characteristic
    HeadingCharacteristic.addDescriptor(NotifyDescriptor);
    LatCharacteristic.addDescriptor(NotifyDescriptor);
    LonCharacteristic.addDescriptor(NotifyDescriptor);
    BatteryCharacteristic.addDescriptor(NotifyDescriptor);
    LeftFlywheelRPMCharacteristic.addDescriptor(NotifyDescriptor);
    RightFlywheelRPMCharacteristic.addDescriptor(NotifyDescriptor);
    PersonDetectedCharacteristic.addDescriptor(NotifyDescriptor);
    CommandFlywheelRPMCharacteristic.addDescriptor(NotifyDescriptor);
    CommandYawCharacteristic.addDescriptor(NotifyDescriptor);
    PitchRPMCharacteristic.addDescriptor(NotifyDescriptor);


    // init the service
    BLE.addService(sensorService);

    // write initial values
    HeadingCharacteristic.writeValue(0.0);
    LonCharacteristic.writeValue(0.0);
    LatCharacteristic.writeValue(0.0);
    BatteryCharacteristic.writeValue(0);
    LeftFlywheelRPMCharacteristic.writeValue(0);
    RightFlywheelRPMCharacteristic.writeValue(0);
    PersonDetectedCharacteristic.writeValue(false);
    CommandFlywheelRPMCharacteristic.writeValue(0.0);
    CommandYawCharacteristic.writeValue(0.0);

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
 * @brief In the interest of not spamming of the connected Bluetooth device, we only send
 * updated values when the newly read value exceeds a certain threshold when compared with
 * the previously sent value.
 */
bool ShouldSendData(float val, SavedData prevVal) {
  return prevVal.val + prevVal.threshold > val || prevVal.val - prevVal.threshold < val;
}

/**
 * @brief Updates sensor service's characteristics to fit currently read sensor
 * values
 */
void UpdateSensorService() {

  // verify device does not have people in front of it
  if (IsPersonDetected()) {
    Monitor.println("PERSON DETECTED!!!!");
    PersonDetectedCharacteristic.setValue(true);
  }
  else {
    PersonDetectedCharacteristic.setValue(false);
  }


  // get values
  float heading = GetHeading();

  GPSData pos = GetGPSData();


  // print values
  if (PRINTING_SENSOR_DATA) {
    Monitor.println("HEADING: " + String(heading));
    Monitor.println("LAT: " + String(pos.lat));
    Monitor.println("LON: " + String(pos.lon));
    Monitor.println("\r\n\r\n");
  }


  // should we send the data
  // if (ShouldSendData(heading, SavedState.heading)) {
    HeadingCharacteristic.setValue(heading);
  // }

  // if (ShouldSendData(pos.lon, SavedState.lon)) {
    LonCharacteristic.setValue(pos.lon);
  // }

  // if (ShouldSendData(pos.lat, SavedState.lat)) {
    LatCharacteristic.setValue(pos.lat);
  // }
  //
  LeftFlywheelRPMCharacteristic.setValue(GetRPM(LEFT));
  RightFlywheelRPMCharacteristic.setValue(GetRPM(RIGHT));

}

/**
 * @brief Checks if the Bluetooth app has sent any commands and executes
 * whatever has been received
 */
void ExecuteCommands() {
  if (CommandFlywheelRPMCharacteristic.written()) {
    double raw_val = CommandFlywheelRPMCharacteristic.value();
    delay(100);

    Monitor.println(raw_val);
    Monitor.flush();
    StartMotors(raw_val);
  }

  if (CommandYawCharacteristic.written()) {
    double yaw = CommandYawCharacteristic.value();
    delay(100);
    Monitor.print("YAW: ");
    Monitor.println(yaw);
    Monitor.flush();
  }

}
