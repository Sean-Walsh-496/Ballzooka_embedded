"""
Simple test script that let's your computer act as a BLE (Bluetooth Low Energy) 
client. Enables you to debug BLE on Arduino from your PC.
"""

# constants
SENSOR_SERVICE_UUID = "ba10f731-f94d-45f8-8ccd-89e393b418f4"
TEST_CHAR_UUID = "ba10f732-f94d-45f8-8ccd-89e393b418f4"

import asyncio
from bleak import BleakScanner, BleakClient

async def find_ballzooka():
    """
    scans for ballzooka's specific service UUID defined in the Bluetooth_Handler
    file.
    """
    SCAN_TIME = 5.0
    print(f"Scanning for {SCAN_TIME} seconds, please wait...")
    candidates = await BleakScanner.discover(SCAN_TIME, service_uuids=[SENSOR_SERVICE_UUID])
    
    try:
        return candidates[0] # discover returns a list of devices
    except:
        return False


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
        
        print("Getting services...", flush=True)
        services = await client.get_services()

        print("Offered services:", flush=True)
        for service in services:
            print(f"{service} (UUID={service.uuid})", flush=True)
            for char in service.characteristics:
                print(f"\t{char} (UUID={char.uuid})", flush=True)

        test_char = await client.read_gatt_char(TEST_CHAR_UUID)
        print(test_char, flush=True)

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
    