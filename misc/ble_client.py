"""
Simple test script that let's your laptop act as a BLE (Bluetooth Low Energy) 
client. Enables you to debug BLE on Arduino.
"""

import asyncio
from bleak import BleakScanner

async def scan_for_devices():
    SCAN_TIME = 5.0
    print(f"Scanning for {SCAN_TIME} seconds, please wait...")
    devices = await BleakScanner.discover(SCAN_TIME, service_uuids=["ba10f731-f94d-45f8-8ccd-89e393b418f4"])
    for device in devices:
        print(device)

if __name__ == "__main__":
    asyncio.run(scan_for_devices())    