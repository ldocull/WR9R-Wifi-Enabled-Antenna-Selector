#
# WR9R Auto Band Switch Manager
# This will automatically select the antenna based on band selected
# on the K3
# 01-01-2023   Larry D O'Cull    WR9R
#

import serial
import requests
import time
import tkinter as tk
import tkinter.font as tkFont
import winsound


MY_VERSION = "WR9R  V0.311w"
MY_SWITCH_URL = "http://192.168.1.179/"
MY_K3_COMM_PORT = "COM4"
MY_KAT500_COMM_PORT = "COM13"
MY_COMM_RATE = "38400"
MY_AUTOTUNE_ENABLE = "Y"    # Y = Enabled

## This is the WR9R K3-KAT500-WifiRemote Switching Antenna operating situation

band_LUT = [
# K3_string, switch_pos, tune_secs, KAT500_string
    [b'BN00',   "FIVE",    0,  b'AN3;MDB;'],    #160M   Dummy load for safety
    [b'BN01',   "THREE",   4,  b'AN2;MDM;T;'],  # 80M   Dipole
    [b'BN02',   "THREE",   3,  b'AN1;MDB;'],    # 60M   Vertical
    [b'BN03',   "THREE",   3,  b'AN1;MDB;'],    # 40M   Vertical
    [b'BN04',   "ONE",     4,  b'AN1;MDB;'],    # 30M   Vertical
    [b'BN05',   "TWO",     1,  b'AN2;MDM;'],    # 20M   HexBeam
    [b'BN06',   "TWO",     1,  b'AN2;MDM;'],    # 17M   HexBeam
    [b'BN07',   "TWO",     1,  b'AN2;MDM;'],    # 15M   HexBeam
    [b'BN08',   "TWO",     1,  b'AN2;MDM;'],    # 12M   HexBeam
    [b'BN09',   "TWO",     1,  b'AN2;MDM;'],    # 10M   HexBeam
    [b'BN10',   "TWO",     1,  b'AN2;MDM;']     #  6M   HexBeam
]
# other possibilities..
#    [b'BN01',   "THREE",   8,  b'AN2;MDM;T;'],  # 75M   Dipole tough-tune for 75M
#    [b'BN01',   "THREE",   4,  b'AN1;MDB;'],    # 80M   Vertical
#    [b'BN02',   "THREE",   5,  b'AN2;MDM;T;'],  # 60M   Dipole
#    [b'BN05',   "TWO",     3,  b'AN1;MDB;'],    # 20M   Vertical


tunerConfig = b''

#  the main window
window = tk.Tk()
window.geometry("400x200")
window.title(MY_VERSION)

#  a variable to store the checkbox value and set the default value
full_auto = tk.IntVar(value=1)
#  the checkbox
full_auto_checkbox = tk.Checkbutton(window, text="FullAuto", variable=full_auto)
full_auto_checkbox.pack(side="top")

#  a frame to hold the checkbox and caption
frame = tk.Frame(window)
frame.pack()
#  a label to display the caption and pack it into the frame
caption_font = tkFont.Font(size=20, weight="bold")
caption_label = tk.Label(frame, text="", font=caption_font)
caption_label.pack(side="top")

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
                if (tunerConfig[2] == ord('1')):
                    option1.select()
                if (tunerConfig[2] == ord('2')):  # set radio button to reflect
                    option2.select()                # button change will update KAT500
                if (tunerConfig[2] == ord('3')):
                    option3.select()                    
                return band_LUT[i][j+2]
    # Return None if the string is not found
    return None

# Apply low-power TUNE signal to allow antenna tuners to adjust
def tune_default(secs):
    print("Tuning...")
    time.sleep(1)            
    K3ser.write(b"SWH16;")
    time.sleep(secs)            
    K3ser.write(b"SWH16;")
    print("Complete")

def band_poll_K3():
    time.sleep(0.25)
    K3ser.flushInput()
    K3ser.write(b"BN;")   # request band currently selected
    time.sleep(0.25)

def freq_poll_K3():
    time.sleep(0.25)
    K3ser.flushInput()
    K3ser.write(b"FA;")   # request the current dial frequency
    time.sleep(0.25)

# function to open the popup window
def tune_popup(secs):
    winsound.Beep(440, 500)
    #  the popup window
    popup = tk.Toplevel()
    popup.title("ALERT")
    popup.geometry("100x50")

    # function to be called when the "TUNE!" button is clicked
    def tune():
        print("Tuning up for %d.." % secs)
        time.sleep(1)            
        K3ser.write(b"SWH16;")
        time.sleep(secs)            
        K3ser.write(b"SWH16;")
        print("Complete")
        popup.destroy()    
    
    # "TUNE!" button
    tune_button = tk.Button(popup, text="TUNE!", command=tune)
    tune_button.pack(side="left")

    # "Exit" button
    exit_button = tk.Button(popup, text="Exit", command=popup.destroy)
    exit_button.pack(side="right")


def setWR9R(data):
    reply = get_band_select(data)
    if reply != None:
        response = requests.get(url + reply)
    else:
        print("No Change") # botched string handling

    if response == 0:
        print("+")
    elif response.status_code == 200:
        print("Success!")
        # Now issue a 'Tune' to set auto-antenna tuning
        tuningTime = get_tune_request(data)  # get desired settings
        if MY_AUTOTUNE_ENABLE == "Y" and tuningTime != 0:
            if full_auto.get() == 1:
                tune_default(tuningTime)
            else:
                tune_popup(tuningTime)

                
    elif response.status_code == 404:
        print("Resource not found!")
    else:
        print("Error!")
   
last_data = b''

def station_service():

    global last_data
    global url
    
    # Write a message to the serial port
    junkstr = b''
    while True:
        i = 0
        while K3ser.inWaiting() != 0:
            char = K3ser.read(1)
            junkstr += char
            # Check if the character is the end of the message
            if char == b';': # is there a completed message for somewhere
                break        # it's best to jump on the back of the train
            i += 1
            if (i > 250):
                break
        break
    
    print ('[' + junkstr.decode() + ']')

    band_poll_K3()    # add a car to the train
    
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
        if (i > 250):
            break
      #  time.sleep(0.05)

    # Validate the response before going further..
    if ((data!=last_data) and (len(data.decode())>=4) and (data[0]==ord('B')) and (data[1]==ord('N'))):
        response = 0
        caption_label.config(text="{"+data.decode()+"}")
        # Now set the WR9R antenna switch based on newly selected band
        setWR9R(data)
        last_data = data

    window.after(100, station_service)    
    # 1-second poll time is quick enough


##
###   BEGINNING OF MAIN CODE SECTION
##
    
#wait for virtual ports to open on startup..
print (MY_VERSION)
time.sleep(5)

# Open the serial port at 9600 baud rate
K3ser = serial.Serial(MY_K3_COMM_PORT, baudrate=MY_COMM_RATE)   # K3 Connect
KAT500ser = serial.Serial(MY_KAT500_COMM_PORT, baudrate=MY_COMM_RATE) #KAT500 Connect
band_poll_K3()      # prime the system at startup with a band request

#  a variable to store the selected radio button value
selected_radio = tk.StringVar()

#  a function to run when the "Tune" button is clicked
def tune():
    # Get the value of the selected radio button
    selected = selected_radio.get()
    # Print a message indicating which radio button is selected
    print(f"Tuning to option {selected}")
    tune_default(3)

#  a function to run when the "Exit" button is clicked
def exit_app():
    window.destroy()

#  a frame to hold the radio buttons
radio_frame = tk.Frame(window)
radio_frame.pack(side="left", padx=20)

# Function that is called on radio button selections
def button_change():
    if (selected_radio.get() == "Option 1"):
        KAT500ser.write(b'AN1;MDM;')
    if (selected_radio.get() == "Option 2"):
        KAT500ser.write(b'AN2;MDM;')
    if (selected_radio.get() == "Option 3"):
        KAT500ser.write(b'AN3;MDM;')
        
#  a radio button with the value "Option 1"
option1 = tk.Radiobutton(radio_frame, text="ANT-1", variable=selected_radio, value="Option 1", command=button_change)
option1.pack()

#  a radio button with the value "Option 2"
option2 = tk.Radiobutton(radio_frame, text="ANT-2", variable=selected_radio, value="Option 2", command=button_change)
option2.pack()

#  a radio button with the value "Option 3"
option3 = tk.Radiobutton(radio_frame, text="ANT-3", variable=selected_radio, value="Option 3", command=button_change)
option3.pack()

#  a frame to hold the "Tune" and "Exit" buttons
button_frame = tk.Frame(window)
button_frame.pack(side="right")

#  a "Tune" button
tune_button = tk.Button(button_frame, text=" Tune ", command=tune)
tune_button.pack(side="top", padx=20, pady=10)

#  an "Exit" button
exit_button = tk.Button(button_frame, text=" Exit ", command=exit_app)
exit_button.pack(side="bottom")

option1.select()
KAT500ser.write(b'AN1;MDM;')    # starting KAT500 state

# Run the main loop
window.after(1000, station_service)

# Run the main loop
window.mainloop()

# Close the serial port
K3ser.close()
KAT500ser.close()
# Fin
