/*
    WR9R Antenna Switch Wifi server firmware..
    this package runs native on ESP32 and provides the web
    interface for the Water Softener Level Dection product.

    Mar 2020
    -Larry D O'Cull -- WR9R   (c) 2020

    
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.    
*/

#ifndef ASWIFI_H_
#define ASWIFI_H_

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define DEBUG_PORT Serial
#define HOST_PORT Serial2
#define RADIO_PORT HOST_PORT


const int led = 13;
const int RXD2_pin = 16;
const int TXD2_pin = 17;
const int openAnalogPin = 34;

// Assign output variables to GPIO pins
const int output5 = 26;
const int output4 = 25;
const int output3 = 33;
const int output2 = 32;
//const int output1 = 35;   // doesn't work as ouput!
const int output1 = 27;

// Assign inputs variables to GPIO pins
const int input5 = 18;
const int input4 = 19;
const int input3 = 21;
const int input2 = 22;
const int input1 = 23;

#define MAX_LIST_LEN 50
#define JSON_ELEMENTS 6

extern const char SSIDformhtml[];
extern const char NAMESformhtml[];
extern const char NAMESendhtml[];
extern const char FREQSformhtml[];
extern const char FREQSendhtml[];


struct SITEDATA {
    char nameTag[32];
    float val;
    int unitType;
};

extern char myIPstr[40];

extern String tableOutput;
extern String JSONOutput;
extern String SiteDatahtml;
extern String SiteEndhtml;

extern String AntennaSiteHeader;
extern String ButtonHeader;
extern String ButtonBody;
extern String ButtonFooter;
extern String AntennaFooter;

void handleRoot();
void Output_Change_ONE();
void Output_Change_TWO();
void Output_Change_THREE();
void Output_Change_FOUR();
void Output_Change_FIVE();
void Output_Change(int);

char *trim(char *s);   // trim ends of spaces..

#endif 
