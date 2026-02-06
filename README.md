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
Android tablet acts as our central device.

The Arduino's BLE capabilities are implemented in __Bluetooth_Handler.cpp__.
