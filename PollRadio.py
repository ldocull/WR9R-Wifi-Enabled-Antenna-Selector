# WR9R Auto Band Switch Manager
# This will automatically select the antenna based on band selected
# on the K3
# 12-27-2022   Larry D O'Cull    WR9R
#

import serial
import requests
import time

MY_SWITCH_URL = "http://192.168.1.179/"
MY_COMM_PORT = "COM4"
MY_COMM_RATE = "38400"

band_LUT = [
    [b'BN00',"FIVE"],   #160M
    [b'BN01',"THREE"],  # 80M
    [b'BN02',"THREE"],  # 60M
    [b'BN03',"THREE"],  # 40M  
    [b'BN04',"THREE"],  # 30M
    [b'BN05',"TWO"],    # 20M
    [b'BN06',"TWO"],    # 17M
    [b'BN07',"TWO"],    # 15M
    [b'BN08',"TWO"],    # 12M
    [b'BN09',"TWO"],    # 10M
    [b'BN10',"TWO"]     #  6M
]

# Define a function to search for a string in the array
def search_array(string):
    for i in range(len(band_LUT)):
        for j in range(len(band_LUT[i])):
            if band_LUT[i][j] == string:
                # Return the value at the next column
                return band_LUT[i][j+1]
    # Return None if the string is not found
    return None

# Apply low-power TUNE signal to allow antenna tuners to adjust
def tune():
    print("Tuning...")
    time.sleep(1)            
    ser.write(b"SWH16;")
    time.sleep(4)            
    ser.write(b"SWH16;")
    print("Complete")
    return None

def setWR9R(data):
        reply = search_array(data)
        if reply != None:
            response = requests.get(url + reply)
        else:
            print("No Change") # botched string handling

        if response == 0:
            print("+")
        elif response.status_code == 200:
            print("Success!")
            # Now issue a 'Tune' to set auto-antenna tuning
            tune()
        elif response.status_code == 404:
            print("Resource not found!")
        else:
            print("Error!")
        return None
            
#wait for virtual ports to open on startup..
time.sleep(5)
# Open the serial port at 9600 baud rate
ser = serial.Serial(MY_COMM_PORT, baudrate=MY_COMM_RATE)

last_data= b''

while True:
    # Write a message to the serial port
    ser.flushInput()
    ser.write(b"BN;")
    time.sleep(0.25)
    
    # Read characters from the serial port until a certain condition is met
    data = b''
    url = MY_SWITCH_URL

    while True:
        # Read a single character
        char = ser.read(1)
        # Check if the character is the end of the message
        if char == b';':
            break
        # Add the character to the data
        data += char

    # Validate the response before going further..
    if data!=last_data and len(data)==4 and data[0]==b'B' and data[1]==b'N':
        response = 0
        print("{"+data+"}")
        # Now set the WR9R antenna switch based on newly selected band
        setWR9R(data)
        last_data = data
        
    # 2-second poll time is quick enough
    time.sleep(2)
    
    # Print the data
##    print(data + " / " + last_data)

# Close the serial port
ser.close()

# Fin
