#
# WR9R Auto Band Switch Manager
# This will automatically select the antenna based on band selected
# on the K3
# 12-29-2022   Larry D O'Cull    WR9R
#

import serial
import requests
import time

MY_SWITCH_URL = "http://192.168.1.179/"
MY_COMM_PORT = "COM4"
MY_COMM_RATE = "38400"
MY_AUTOTUNE_ENABLE = "Y" # Y = Enabled

band_LUT = [        # K3_string, switch_pos, tune_required_seconds
    [b'BN00',"FIVE",0],   #160M
    [b'BN01',"THREE",0],  # 80M
    [b'BN02',"THREE",0],  # 60M
    [b'BN03',"THREE",2],  # 40M  
    [b'BN04',"THREE",3],  # 30M
    [b'BN05',"TWO",3],    # 20M
    [b'BN06',"TWO",0],    # 17M
    [b'BN07',"TWO",0],    # 15M
    [b'BN08',"TWO",0],    # 12M
    [b'BN09',"TWO",0],    # 10M
    [b'BN10',"TWO",0]     #  6M
]

# Match the K3 string to the desired band from the LUT
def get_band_select(string):
    for i in range(len(band_LUT)):
        for j in range(len(band_LUT[i])):
            if band_LUT[i][j] == string:
                # Return the value at the next column
                return band_LUT[i][j+1]
    # Return None if the string is not found
    return None

# Match the K3 string to a tune-request per the LUT
def get_tune_request(string):
    for i in range(len(band_LUT)):
        for j in range(len(band_LUT[i])):
            if band_LUT[i][j] == string:
                # Return the value at the next column
                return band_LUT[i][j+2]
    # Return None if the string is not found
    return None


# Apply low-power TUNE signal to allow antenna tuners to adjust
def tune(secs):
    print("Tuning...")
    time.sleep(1)            
    ser.write(b"SWH16;")
    time.sleep(secs)            
    ser.write(b"SWH16;")
    print("Complete")
    return None

def setWR9R(data):
        reply = get_band_select(data)
        tune_request  = get_tune_request(data)
        if reply != None:
            response = requests.get(url + reply)
        else:
            print("No Change") # botched string handling

        if response == 0:
            print("+")
        elif response.status_code == 200:
            print("Success!")
            # Now issue a 'Tune' to set auto-antenna tuning
            if MY_AUTOTUNE_ENABLE == "Y":
                if tune_request != 0:
                    tune(tune_request)
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
    while True:
        # Read a single character looking for end of last message on bus
        char = ser.read(1)
        # Check if the character is the end of the message
        if char == b';':
            break
    time.sleep(0.25)
    ser.flushInput()    # flush the buffer
    ser.write(b"BN;")   # add a car to the train
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

# Close the serial port
ser.close()

# Fin
