# WR9R Auto Band Switch Manager
# This will automatically select the antenna based on band selected
# on the K3
# 12-27-2022   Larry D O'Cull    WR9R
#

import serial
import requests
import time

MY_SWITCH_URL = "http://192.168.1.179/"
# Open the serial port at 9600 baud rate
ser = serial.Serial("COM4", baudrate=38400)

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

    if data!=last_data and len(data)==4:

        response = 0
        
        # Now set the WR9R switch based on current band
        if data == b'BN00':
            response = requests.get(url + "ONE")   #160M
        elif data == b'BN01':
            response = requests.get(url + "THREE") #80M
        elif data == b'BN02':
            response = requests.get(url + "THREE") #60M
        elif data == b'BN03':
            response = requests.get(url + "THREE") #40M
        elif data == b'BN04':
            response = requests.get(url + "THREE") #30M
        # elif data == b'BN10':
        #    response = requests.get(url + "THREE") #6M (on the dipole)
        elif data == b'BN05' or data == b'BN06' or data == b'BN07' or data == b'BN08' or data == b'BN09' or data == b'BN10':
            response = requests.get(url + "TWO") #20M-6M (on the beam)
        else:
            print("No Change") #

        if response == 0:
            print("+")
        elif response.status_code == 200:
            print("Success!")
        elif response.status_code == 404:
            print("Resource not found!")
        else:
            print("Error!")

        last_data = data

    time.sleep(2)
    
    # Print the data
    print(data + " / " + last_data)

# Close the serial port
ser.close()

# Fin
