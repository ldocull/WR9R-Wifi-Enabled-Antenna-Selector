#
# WR9R Auto Band Switch Manager
# This will automatically select the antenna based on band selected
# on the K3
# 01-01-2023   Larry D O'Cull    WR9R
#

import serial
import requests
import time

MY_SWITCH_URL = "http://192.168.1.179/"
MY_K3_COMM_PORT = "COM4"
MY_KAT500_COMM_PORT = "COM13"
MY_COMM_RATE = "38400"
MY_AUTOTUNE_ENABLE = "Y" # Y = Enabled

## This is the WR9R operating situation
band_LUT = [
# K3_string, switch_pos, tune_secs, KAT500_string
    [b'BN00',   "FIVE",    0,  b'AN3;MDB;'],    #160M   Dummy load for safety
    [b'BN01',   "THREE",   4,  b'AN2;MDM;T;'],  # 80M   Dipole
#    [b'BN01',   "THREE",   8,  b'AN2;MDM;T;'],  # 75M   Dipole tough-tune for 75M
#    [b'BN01',   "THREE",   4,  b'AN1;MDB;'],    # 80M   Vertical
    [b'BN02',   "THREE",   5,  b'AN1;MDM;'],    # 60M   Vertical
#    [b'BN02',   "THREE",   5,  b'AN2;MDM;T;'],  # 60M   Dipole
    [b'BN03',   "THREE",   3,  b'AN1;MDM;'],    # 40M   Vertical
    [b'BN04',   "THREE",   5,  b'AN1;MDM;'],    # 30M   Vertical
#    [b'BN05',   "TWO",     3,  b'AN1;MDB;'],    # 20M   Vertical
    [b'BN05',   "TWO",     3,  b'AN2;MDM;'],    # 20M   HexBeam
    [b'BN06',   "TWO",     3,  b'AN2;MDM;'],    # 17M   HexBeam
    [b'BN07',   "TWO",     3,  b'AN2;MDM;'],    # 15M   HexBeam
    [b'BN08',   "TWO",     3,  b'AN2;MDM;'],    # 12M   HexBeam
    [b'BN09',   "TWO",     3,  b'AN2;MDM;'],    # 10M   HexBeam
    [b'BN10',   "TWO",     3,  b'AN2;MDM;']     #  6M   HexBeam
]

tunerConfig = b''

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
                tunerConfig = band_LUT[i][j+3]
                print(tunerConfig)
                KAT500ser.write(tunerConfig) # set desired configuration on KAT500
                return band_LUT[i][j+2]
    # Return None if the string is not found
    return None


# Apply low-power TUNE signal to allow antenna tuners to adjust
def tune(secs):
    print("Tuning...")
    time.sleep(1)            
    K3ser.write(b"SWH16;")
    time.sleep(secs)            
    K3ser.write(b"SWH16;")
    print("Complete")
    return None

def band_poll_K3():
    time.sleep(0.25)
    K3ser.flushInput()
    K3ser.write(b"BN;")   # request band currently selected
    time.sleep(0.25)
    return None

def freq_poll_K3():
    time.sleep(0.25)
    K3ser.flushInput()
    K3ser.write(b"FA;")   # request the current dial frequency
    time.sleep(0.25)
    return None

def setWR9R(data):
    reply = get_band_select(data)
    tune_request  = get_tune_request(data)  # get desired settings
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
print "01-01-2023  WR9R"
time.sleep(5)

# Open the serial port at 9600 baud rate
K3ser = serial.Serial(MY_K3_COMM_PORT, baudrate=MY_COMM_RATE)   # K3 Connect
KAT500ser = serial.Serial(MY_KAT500_COMM_PORT, baudrate=MY_COMM_RATE) #KAT500 Connect
band_poll_K3()      # prime the system at startup with a band request

last_data = b''

while True:
    # Write a message to the serial port
    i = 0
    junkstr = b''
    while True:
        while K3ser.inWaiting() != 0:
            char = K3ser.read(1)
            junkstr += char
            # Check if the character is the end of the message
            if char == b';': # is there a completed message for somewhere
                break        # it's best to jump on the back of the train
        time.sleep(0.25)
        i = i + 1
        if i > 10:
            break;  

    print "[" + junkstr + "]"

    band_poll_K3()    # add a car to the train
    
    # Read characters from the serial port until a certain condition is met
    data = b''
    url = MY_SWITCH_URL

    while K3ser.inWaiting() != 0:
        char = K3ser.read(1)       # Read a single character
        # Check if the character is the end of the message
        if char == b';':
            break
        # Add the character to the data
        data += char
        time.sleep(0.25)

##    print data
    
    # Validate the response before going further..
    if data!=last_data:
        if data[0]==b'B' and data[1]==b'N':
            response = 0
            print("{"+data+"}")
            # Now set the WR9R antenna switch based on newly selected band
            setWR9R(data)
##            freq_poll_K3()
            last_data = data
        
    # 2-second poll time is quick enough
    time.sleep(2)

# Close the serial port
K3ser.close()
KAT500ser.close()
# Fin
