from arduino.app_utils import *

def analyze_thermal_data(rows: int, cols: int, data: str) -> bool:
    data = [float(val) for val in data.split(",")] # convert string to array

    assert rows * cols == len(data) # ensure args match up
    
    return False


if __name__ == "__main__":
    Bridge.provide("analyze_thermal_data", analyze_thermal_data)

    App.run()