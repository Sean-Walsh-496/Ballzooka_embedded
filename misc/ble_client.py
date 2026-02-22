"""
Simple test script that let's your computer act as a BLE (Bluetooth Low Energy) 
client. Enables you to debug BLE on Arduino from your PC.
"""

# constants
UUIDs = {
    "sensor_service":           "ba10f731-f94d-45f8-8ccd-89e393b418f4",
    "heading_characteristic":   "ba10f732-f94d-45f8-8ccd-89e393b418f4",
    "lat_characteristic":       "ba10f733-f94d-45f8-8ccd-89e393b418f4",
    "lon_characteristic":       "ba10f736-f94d-45f8-8ccd-89e393b418f4",
    "battery_characteristic":   "ba10f734-f94d-45f8-8ccd-89e393b418f4",
    "RPM_characteristic":       "ba10f735-f94d-45f8-8ccd-89e393b418f4",
}

import asyncio
from bleak import BleakScanner, BleakClient

async def find_ballzooka():
    """
    scans for ballzooka's specific service UUID defined in the Bluetooth_Handler
    file.
    """
    SCAN_TIME = 5.0
    print(f"Scanning for {SCAN_TIME} seconds, please wait...")
    candidates = await BleakScanner.discover(SCAN_TIME, service_uuids=[UUIDs["sensor_service"]])
    
    try:
        return candidates[0] # discover returns a list of devices
    except:
        return False

def check_services(client):
    """
    Prints out a list of available services and their UUIDs
    """
    print("Getting services...", flush=True)
    services = client.services

    print("Offered services:", flush=True)
    for service in services:
        print(f"{service}", flush=True)
        for idx, char in enumerate(service.characteristics):
            print(f"\t{idx + 1}. {char} ", flush=True)


async def main():
    """
    A basic main function which finds the ballzooka Arduino and performs some 
    test operations.
    """
    ballzooka = await find_ballzooka()
    if not ballzooka:
        raise Exception("ERROR: could not find ballzooka")
    print("Ballzooka found")

    client = BleakClient(ballzooka)
    
    try:
        await client.connect()
        if not client.is_connected:
            print("Connection failed")
            return
        print("Connection established", flush=True)
        
        check_services(client)

        heading = await client.read_gatt_char(UUIDs["heading_characteristic"])
        lat = await client.read_gatt_char(UUIDs["lat_characteristic"])
        lon = await client.read_gatt_char(UUIDs["lon_characteristic"])
        print(f"HEADING: {int.from_bytes(heading, byteorder="little") / 100}", flush=True)
        print(f"LAT: {int.from_bytes(lat, byteorder="little") / 1000}", flush=True)
        print(f"LON: {int.from_bytes(lon, byteorder="little") / 1000}", flush=True)

    except asyncio.TimeoutError:
        print("ERROR: Async timeout")

    except KeyboardInterrupt:
        print("ERROR: CTRL+C detected")
    
    finally:
        print("Disconnecting")
        if client.is_connected:
            await client.disconnect()

if __name__ == "__main__":
    asyncio.run(main())
    