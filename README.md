
# WR9R-Wifi-Enabled-Antenna-Selector
This PCBA provides a method to remotely control antenna selection within a LAN using a standard browser.  I like to operate my HAM station, which is located in the basement, from other areas in or around my home.  This PCB and Firmware upgrade was easy to apply to my existing [Ameritron RCS-8V](https://mfjenterprises.com/products/rcs-8v) Antenna Selector control box while still maintaining manual control when I am in the basement radio-shack. 

## Key features:

- Directly fits within the existing enclosure
- Supports original manual control but adds remote control 
  (Changing the antenna choice at the front panel at any time, resets the device to the manual selection mode)
- Browser buttons can be named through the browser to support antenna upgrades


![Typical Antenna Remote Controller Box](https://imgur.com/vl0tTJB.jpg)

![Installation](https://imgur.com/1aA2Qp5.jpg)

## Assembly Installation:

 The control wires were clipped from the back terminal strip and landed to the rear of the PCBA as shown above.
- New wires were the placed from the "To Relays" header to the terminal strip
- 12VDC power was picked up at the DC jack on the rear of the enclosure -- but could be picked up after the from panel switch on the front PCBA.

![Main Operating Screen](https://imgur.com/COa3D5a.jpg)

## Setup:

 The software can be modified and hard coded to your network. Otherwise you can set it up using a phone or other internet-ready device. Connect to the Wifi hotspot generated by the Antenna Selector interface that looks like:  *WR9RasNNN*,  where *NNN* is a somewhat random number.
- Password is '*12345678*'
- On the same device, use a browser to then go to the host address:  https://192.168.4.1/SSID

![enter image description here](https://imgur.com/8Ln4B3V.jpg)

- There, enter the SSID and Password for the network you wish to use within your LAN
- If you're plugged into the wifi module of the new PCBA with a USB cable, you can use a terminal session to see the new IP.  Otherwise. you will need to log into your router to find the new address assigned to the device.

## Antenna Names:
The buttons on the main screen can be renamed at any time. Just go to **htpps://*yourIPadd*/NAMES**

![enter image description here](https://imgur.com/sdjbDu8.jpg)

73
-[WR9R](http://wr9r.com/) 

