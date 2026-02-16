# Ballzooka Embedded Code

Contains all source code and documentation related to the ballzooka's embedded
software

## Installation
The "/embedded" directory contains all Arduino source. Opening this in the 
Arduino IDE (tested and ran with IDE 2.0) will allow you to compile the source
and upload it to an Arduino device. The software is targetting the Arduino UNO 
Q.



## Implementation Details

### Bluetooth
Ballzooka relies on the Bluetooth Low Energy (BLE) protocol to support 
communication between the Android tablet and the cannon. BLE uses a 
*peripheral / central-device architecture*. The peripheral is the more resource 
constrained device, in this case the Arduino inside the Ballzooka, while the 
Android tablet acts as our central device. This is similar to a client-server
architecture, where the peripheral acts as a server.

The Arduino's BLE capabilities are implemented in __Bluetooth_Handler.cpp__.

The peripheral device can advertise several *services*, which act as logical 
groupings of individual pieces of data. These pieces of data, which may be read
or written by the central device, are called *characteristics*.

Two services are going to be advertised: the sensor service and the command 
service.
- The _Sensor Service_ will be a read-only service that, when requested from, 
  returns the latest readings from relevant sensors to the Android application.
- The _Command Service_ will be a read-write service that allows the application
  to write commands to the Ballzooka, such as a target location.

A heartbeat signal is sent periodically from the Ballzooka to the app to ensure 
that the connection is still active. In order to save power, once a connection 
is established, the Arduino-side of the BLE connection is entirely passive until
a request is sent by the app. If the app crashes or gets outside of the BLE's 
range, the Arduino will not know. For this reason, a heartbeat is sent to ensure
the connection is stable, and if a connection is unexpectedly lost the Ballzooka
can safely react.

### Sensors
Multiple sensors can be found on the Ballzooka, the majority of which are I2C 
devices driven on a common serial bus. These connected directly to the builtin
SCL and SDA GPIO pins are the Arduino.

#### Sonar
The sonar sensor is a I2CXL-MaxSonar-EZ. It's using the devices default I2C 
address of 0x70. This is in the 7-bit format, and Arduino's Wire library 
automatically handles appending a read or write bit the address during 
communication.

Reading the sensor involves two steps. First the Arduino issue a READ command to 
the device by writing 81 to the sonar. We wait about 100ms (although the 
datasheet indicates that this could be considerably reduced, this is just a test
value) and then read two bytes from the device. These two bytes are appended
into a single integer which represents the read distance in CM.



#### Magnetometer

