/*
    Water Softener Wifi server firmware..
    this package runs native on ESP32 and provides the web
    interface for the Water Softener Level Dection product.

    DEBUG_PORT messages are transponded between main controller and 
    this module  - way more efficient/solid than stock AT-command setup.
    
    Mar 2020
    -LDO
*/

#ifndef DURAWIFI_H_
#define DURAWIFI_H_

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define DEBUG_PORT Serial
#define HOST_PORT Serial2

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
