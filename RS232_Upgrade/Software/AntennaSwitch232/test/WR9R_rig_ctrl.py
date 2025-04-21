#
# WR9R Auto Band Switch Manager
# This will automatically select the antenna based on band selected
# on the K3
# 01-01-2023   Larry D O'Cull    WR9R
#

import serial
import requests
import time
import winsound
import config

MY_VERSION = "WR9R  V0.311w"
MY_SWITCH_URL = "http://192.168.1.179/"
MY_K3_COMM_PORT = "COM4"
MY_KAT500_COMM_PORT = "COM13"
MY_COMM_RATE = "38400"
MY_AUTOTUNE_ENABLE = "Y"    # Y = Enabled

## This is the WR9R K3-KAT500-WifiRemote Switching Antenna operating situation

band_LUT = [
# K3_string, switch_pos, tune_secs, KAT500_string
    [b'BN00',   "FIVE",    0,  b'AN3;MDB;', "160M"],    #160M   Dummy load for safety
    [b'BN01',   "THREE",   4,  b'AN2;MDM;T;',"80M"],  # 80M   Dipole
    [b'BN02',   "THREE",   3,  b'AN1;MDB;',  "60M"],    # 60M   Vertical
    [b'BN03',   "THREE",   3,  b'AN1;MDB;',  "40M"],    # 40M   Vertical
    [b'BN04',   "ONE",     4,  b'AN1;MDM;',  "30M"],    # 30M   Vertical
    [b'BN05',   "TWO",     1,  b'AN2;MDM;',  "20M"],    # 20M   HexBeam
    [b'BN06',   "TWO",     1,  b'AN2;MDM;',  "17M"],    # 17M   HexBeam
    [b'BN07',   "TWO",     1,  b'AN2;MDM;',  "15M"],    # 15M   HexBeam
    [b'BN08',   "TWO",     1,  b'AN2;MDM;',  "12M"],    # 12M   HexBeam
    [b'BN09',   "TWO",     1,  b'AN2;MDM;',  "10M"],    # 10M   HexBeam
    [b'BN10',   "TWO",     1,  b'AN2;MDM;',  "6M"]     #  6M   HexBeam
]
# other possibilities..
#    [b'BN01',   "THREE",   8,  b'AN2;MDM;T;'],  # 75M   Dipole tough-tune for 75M
#    [b'BN01',   "THREE",   4,  b'AN1;MDB;'],    # 80M   Vertical
#    [b'BN02',   "THREE",   5,  b'AN2;MDM;T;'],  # 60M   Dipole
#    [b'BN05',   "TWO",     3,  b'AN1;MDB;'],    # 20M   Vertical


tunerConfig = b''
last_data = b''
bandName = ""
tripTicker = 0

#  a function to run when the "Tune" button is clicked
def tune():
    # Get the value of the selected radio button
    selected = config._w1.selectedButton.get()
    # Print a message indicating which radio button is selected
    print(f"Tuning to option {selected}")
    tune_default(3)

# Match the K3 string to the desired band from the LUT
def get_band_select(string):
    global bandName
    
    for i in range(len(band_LUT)):
        for j in range(len(band_LUT[i])):
            if band_LUT[i][j] == string:
                # Return the value at the next column
                bandName = band_LUT[i][j+4]
                return band_LUT[i][j+1]
    # Return None if the string is not found
    return None

# Match the K3 string to a tune-request per the LUT
def get_tune_request(string):
    global bandName
    global tunerConfig
        
    for i in range(len(band_LUT)):
        for j in range(len(band_LUT[i])):
            if band_LUT[i][j] == string:
                # Return the value at the next column
                tunerConfig = band_LUT[i][j+3]
                bandName = band_LUT[i][j+4]

                KAT500ser.write(tunerConfig) # set desired configuration on KAT500

                if (tunerConfig[2] == ord('1')):
                    config._w1.Radiobutton1.select()
                if (tunerConfig[2] == ord('2')):  # set radio button to reflect
                     config._w1.Radiobutton2.select()                # button change will update KAT500
                if (tunerConfig[2] == ord('3')):
                     config._w1.Radiobutton3.select()                    
                return band_LUT[i][j+2]
    # Return None if the string is not found
    return None

# set the KAT500 antenna position
def setKat500(selnum):
    if(selnum == 1):
        KAT500ser.write(b'AN1;')
    if(selnum == 2):
        KAT500ser.write(b'AN2;')
    if(selnum == 3):
        KAT500ser.write(b'AN3;')

# Apply low-power TUNE signal to allow antenna tuners to adjust
def tune_default(secs):
    print("Tuning...")
    time.sleep(1)            
    K3ser.write(b"SWH16;")
    time.sleep(secs)            
    K3ser.write(b"SWH16;")
    print("Complete")

# poll the band selection of K3 (this may be happening from outside as well)
def band_poll_K3():
    time.sleep(0.25)
    K3ser.flushInput()
    K3ser.write(b"BN;")   # request band currently selected
    time.sleep(0.25)

# poll VFO A (this may be happening from outside as well)
def freq_poll_K3():
    time.sleep(0.25)
    K3ser.flushInput()
    K3ser.write(b"FA;")   # request the current dial frequency
    time.sleep(0.25)

# Set the WR9R Wifi-Enabled-Switch using HTTP_GET
def setWR9Rswitch(reply, data):
    global url

    if reply != None:
        response = requests.get(url + reply)
    else:
        print("No Change") # botched string handling

    if response == 0:
        print("+")
        
    elif response.status_code == 200:
        print("Success!")

        if (reply == "ONE"):    # set radio buttons accordingly
            config._w1.Radiobutton4.select()
        if (reply == "TWO"):    # set radio buttons accordingly
            config._w1.Radiobutton5.select()
        if (reply == "THREE"):    # set radio buttons accordingly
            config._w1.Radiobutton6.select()
        if (reply == "FOUR"):    # set radio buttons accordingly
            config._w1.Radiobutton7.select()
        if (reply == "FIVE"):    # set radio buttons accordingly
            config._w1.Radiobutton8.select()

        if(data != 0):
            # Now issue a 'Tune' to set auto-antenna tuning
            tuningTime = get_tune_request(data)  # get desired settings
            if MY_AUTOTUNE_ENABLE == "Y" and tuningTime != 0:
                if config._w1.FullAuto.get() == 1:
                    tune_default(tuningTime)
                # else:
                #     tune_popup(tuningTime)

    elif response.status_code == 404:
        print("Resource not found!")
    else:
        print("Error!")

def setWR9R(data):
    reply = get_band_select(data)
    setWR9Rswitch(reply, data)

def switch_change(antenna):
    global _w1

    if (antenna == 1):
        setWR9Rswitch("ONE", 0)
    if (antenna == 2):
        setWR9Rswitch("TWO", 0)
    if (antenna == 3):
        setWR9Rswitch("THREE", 0)
    if (antenna== 4):
        setWR9Rswitch("FOUR", 0)
    if (antenna == 5):
        setWR9Rswitch("FIVE", 0)                        

def station_service():

    global last_data
    global url
    global bandName
    global tripTicker

    # Write a message to the serial port
    junkstr = b''
    tempstr = b'' # frequency place holder
    while True:
        i = 0
        while K3ser.inWaiting() != 0:
            char = K3ser.read(1)
            junkstr += char
            # Check if the character is the end of the message
            if char == b';': # is there a completed message for somewhere
                break        # it's best to jump on the back of the train
            i += 1
            if (i > 250):  # antilockup...
                break
        break
    
    print ('[' + junkstr.decode() + ']')
    # put frequency on the form if it's there...  [FA00007289240;]
    #                                               -> 07.289.240
    if ((len(junkstr.decode())>=14) and (junkstr[0]==ord('F')) and (junkstr[1]==ord('A'))):
        tempstr = junkstr[5:7] + b'.' + junkstr[7:10] + b'.' + junkstr[10:13]
        config._w1.Message2.configure(text=tempstr)
        tripTicker = 0

    band_poll_K3()    # ask for band, sync'd to the tail of the last rcvd message
    
    # Read characters from the serial port until a certain condition is met
    data = b''
    url = MY_SWITCH_URL

    i = 0
    while K3ser.inWaiting() != 0:
        char = K3ser.read(1)       # Read a single character
        # Check if the character is the end of the message
        if char == b';':
            break
        # Add the character to the data
        data += char
        i += 1
        if (i > 250):  # antilockup...
            break
      #  time.sleep(0.05)

    # Validate the response before going further..
    if ((data!=last_data) and (len(data.decode())>=4) and (data[0]==ord('B')) and (data[1]==ord('N'))):
        response = 0
        # caption_label.config(text="{"+data.decode()+"}")
        # Now set the WR9R antenna switch based on newly selected band
        setWR9R(data)
        print("bn:" + bandName)
        config._w1.Message1.configure(text=bandName)
        last_data = data
    tripTicker += 1
    
    if (tripTicker > 5):  # if not hearing a freq from another source then make my own
        freq_poll_K3()
        tripTicker = 0
    
    config._top1.after(500, station_service)    
    # 1/2-second poll time is quick enough

def WR9R_init():    
    #wait for virtual ports to open on startup..
    print (MY_VERSION)
    time.sleep(5)

    # Open the serial port at 9600 baud rate
    global K3ser
    K3ser = serial.Serial(MY_K3_COMM_PORT, baudrate=MY_COMM_RATE)   # K3 Connect
    global KAT500ser
    KAT500ser = serial.Serial(MY_KAT500_COMM_PORT, baudrate=MY_COMM_RATE) #KAT500 Connect

    KAT500ser.write(b'AN1;MDM;')    # starting KAT500 state

    band_poll_K3()      # prime the system at startup with a band request

def WR9R_shutdown():
    # Close the serial port
    K3ser.close()
    KAT500ser.close()
    # Fin    