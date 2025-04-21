/*
    WR9R Antenna Switch Wifi server firmware..
    this package runs native on ESP32 and provides the web
    interface for the WR9R Antenna Selector Server product.
   
    Aug 2020
    -Larry D O'Cull  WR9R  (c)2020

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

#include "AS_Wifi.h"

//
// EEPROM mapping for stored params (Names, SSID, etc.)
//
#define TAG_INIT_0 0xBB
#define TAG_INIT_1 0xAA
#define TAG_ADDRESS 0
char tag[2];
#define SSID_ADDRESS 4
char ssid[64];
#define PSWD_ADDRESS (24+4)
char password[64];
#define ANT_NAME_ADDRESS (24+4+64+4)
#define ANT_ADDR_OFFSET 64
char antennaName[6][ANT_ADDR_OFFSET];
#define ANT_FREQ_ADDRESS (ANT_NAME_ADDRESS+(6*(ANT_ADDR_OFFSET)))
char antennaFreq[6][ANT_ADDR_OFFSET];
#define EEPROM_SIZE (ANT_FREQ_ADDRESS+(6*ANT_ADDR_OFFSET))

char myIPstr[40];
String myIPstring;
WebServer server(80);

volatile int interruptCounter;
int totalInterruptCounter;


int switchStatus = 0;
int lastSwitchStatus = 0;

/* 
    Direct radio monitoring add on for Elecraft K3, Kenwood, etc.
    Apl 19, 2025   LDO

  Comms look like this --->   ;FA00005355170; 
    FA000xx******;
    where xx is megahertz
 if selector is in the SW1 (switchStatus = 16) position -- no changes
 if selector is in the SW5 (switchStatus = 1) position -- then auto changes
 so SW2,S3 not assigned
         SW1  10;                               --- multiband vertical here (30M)
         SW2  14,21,28,29;                      --- Beam is setup here (20,15,10M)
         SW3  03,05,07,18,24,50,51,52,53;       --- multiband dipole here (80,60,40,17,12,6M)
         SW4  ;
         SW5  ;
*/

#define NUM_BANDS 5
#define MAX_VALUES_PER_BAND 16
char band_inputs[200];
int bands[NUM_BANDS][MAX_VALUES_PER_BAND];
int band_lengths[NUM_BANDS];

int parse_band(const char *s, int *arr, int max_elements) {
    int count = 0;
    const char *ptr = s;

    while (*ptr != '\0' && *ptr != ';' && count < max_elements) {
        arr[count++] = strtol(ptr, (char**)&ptr, 10);
        while (*ptr == ',') ptr++;
    }

    return count;
}

int parse_bands(const char *s, int bands[NUM_BANDS][MAX_VALUES_PER_BAND], int *band_lengths) {
    int band = 0;
    const char *ptr = s;

    while (*ptr != '\0' && band < NUM_BANDS) {
        const char *end = strchr(ptr, ';');
        char temp[128];

        if (end) {
            size_t len = end - ptr;
            strncpy(temp, ptr, len);
            temp[len] = '\0';
            ptr = end + 1;
        } else {
            strcpy(temp, ptr);
            ptr += strlen(ptr);
        }

        band_lengths[band] = parse_band(temp, bands[band], MAX_VALUES_PER_BAND);
        band++;
    }

    return band; // Number of bands parsed
}

void setupBandPlan()
{
  int i = 0;
  band_inputs[0] = 0;
  for(i=0; i<NUM_BANDS; i++)
  {
      strcat(band_inputs, antennaFreq[i]); // retrieve freqs for each band
  }
  DEBUG_PORT.printf("INPUT:%s\n", band_inputs);

  parse_bands(band_inputs, bands, band_lengths);

  for (i = 0; i < NUM_BANDS; i++) {
      DEBUG_PORT.printf("Band %d: %d -", i, band_lengths[i]);
      for (int j = 0; j < band_lengths[i]; j++) {
          DEBUG_PORT.printf("%d ", bands[i][j]);
      }
      DEBUG_PORT.printf("\n");
  }
}

int find_band_for_frequency(int megaHZ) {
  for (int i = 0; i < NUM_BANDS; i++) {
      for (int j = 0; j < band_lengths[i]; j++) {
          if (bands[i][j] == megaHZ) {
              return i; // Found the band
          }
      }
  }
  return -1; // Not found
}

/*
  poll for the passing rig traffic and grab the megahertz to determine which antenna is required
  based on the table programmed into the FREQ table via the web settings
  Comms look like this --->   ;FA00005355170; 
    FA000xx******;
    where xx is megahertz
*/
int band_index = 9, last_band_index = -1;

int poll_radio_for_frequency() {
    static char buffer[64];
    static int index = 0;

    while (RADIO_PORT.available()) {
        char c = RADIO_PORT.read();
    // while (DEBUG_PORT.available()) {
    //   char c = DEBUG_PORT.read();

        // Start a new message if 'F' is seen
        if (c == 'F') {
            index = 0;
        }

        // Store the character if there's room
        if (index < sizeof(buffer) - 1) {
            buffer[index++] = c;
            buffer[index] = '\0'; // null-terminate
        } else {
            // If buffer overflows, reset
            index = 0;
        }

        // Check for complete message once ';' is seen
        if (c == ';' && index == 14) {
            // Message should be exactly 14 characters: "FA000mmdddddd;"
            if (strncmp(buffer, "FA000", 5) == 0) {
                char mhz_str[3] = { buffer[5], buffer[6], '\0' };
                int megaHZ = atoi(mhz_str);
                index = 0; // Reset for next message
                return megaHZ;
            } else {
                // Invalid message, reset buffer
                index = 0;
            }
        }
    }

    return 0; // No complete message yet
}

void read_Radio()
{ 
                                  // otherwise follow the web or front panel
  int megaHZ = poll_radio_for_frequency();

  if(switchStatus != 1)
  {
    last_band_index = -1;
    return;   // require the switch in this position for "direct AUTO"
  }

  if(megaHZ > 0)     // nothing to see here if 0
  {
    band_index = find_band_for_frequency(megaHZ);
    if (band_index != -1) {
      DEBUG_PORT.printf("Frequency %d MHz found in band %d\n", megaHZ, band_index);

      if(band_index != last_band_index) // is this a new frequency band ?
      {
        Output_Change((band_index)+1); // if so, set switch accordingly
        last_band_index = band_index;
      }
      else 
        DEBUG_PORT.printf("no change\n");
    } 
    else 
    {
      DEBUG_PORT.printf("Frequency %d MHz not found in any band\n", megaHZ);
    }
  }
}

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//
// String clean up routines
//
char *ltrim(char *s)  // left trim of spaces..
{
    char *start;

    start = s;
    while(isspace(*s))
          s++;
    strcpy(start,s);

    return start;
}

char *rtrim(char *s)  // right trim of spaces
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';

    return s;
}

char *trim(char *s)   // trim ends of spaces..
{
    return rtrim(ltrim(s)); 
}

//
// Save EEPROM string parameters
//
void EEStore(int addr, char *cptr)
{
    int i = 0;

    while(*cptr != 0)      // put a string in EEPROM
    {
      EEPROM.write(addr++, *cptr);
      cptr++;
      if(++i > 63) return;  // prevent damage...
    }
    EEPROM.write(addr, 0); // end the string..
}

//
// Restrore EEPROM string parameters
//
void EERecover(int addr, char *cptr)
{
    do  // recover a string from EEPROM
    {
      *cptr = EEPROM.read(addr++);
    } while (*cptr++ != 0);
    
}

//
// Save a list of settings and commit to EEPROM
//
void saveSettings()
{
        EEStore(SSID_ADDRESS, ssid);
        EEStore(PSWD_ADDRESS, password);

        for(int i=0; i<5; i++)
        {
            EEStore(ANT_NAME_ADDRESS + (ANT_ADDR_OFFSET * (i)), trim(antennaName[i]));
        }

        for(int i=0; i<5; i++)
        {
            EEStore(ANT_FREQ_ADDRESS + (ANT_ADDR_OFFSET * (i)), trim(antennaFreq[i]));
        }

        EEPROM.write(TAG_ADDRESS, TAG_INIT_0);  // tag to show the EEPROM initialized and valid
        EEPROM.write(TAG_ADDRESS+1, TAG_INIT_1);

        EEPROM.commit();
}

//
// recover stored antenna descriptor names if available
//
String antName[6];  // antenna descriptor - name
void getNames()
{
        for(int i=0; i<5; i++)
        {
            EERecover(ANT_NAME_ADDRESS + (ANT_ADDR_OFFSET * (i)), antennaName[i]);
            antName[i] = (String)(trim(antennaName[i]));
        }
}

//
// recover stored antenna freq lists (strings) if available
//
String antFreq[6];  // antenna descriptor - name
void getFreqs()
{ 
        for(int i=0; i<5; i++)
        {
            EERecover(ANT_FREQ_ADDRESS + (ANT_ADDR_OFFSET * (i)), antennaFreq[i]);
            antFreq[i] = (String)(trim(antennaFreq[i]));
            DEBUG_PORT.printf("[%s]\n",trim(antennaFreq[i])); // list what I have to work with,,,
        }
}

//
//  Set names of buttons to antenna descriptors
//
void NAMEchange()
{
  String tempStr;

  if (server.hasArg("ant1") || server.hasArg("ant2") ||
        server.hasArg("ant3") || server.hasArg("ant4") ||
          server.hasArg("ant5"))
  {
      antName[0] = server.arg("ant1");    // new assignments to names...
      antName[1] = server.arg("ant2");
      antName[2] = server.arg("ant3");
      antName[3] = server.arg("ant4");
      antName[4] = server.arg("ant5");

      for(int x=0; x<5; x++)
      {
        strcpy(antennaName[x], antName[x].c_str()); // prep to store in EEPROM
        trim(antennaName[x]);
        antName[x] = (String)antennaName[x];
      }
      saveSettings();
  }
  tempStr = NAMESformhtml;
  tempStr += "<label for=\"ant1\">ANT-1: </label><input type=\"text\" id=\"ant1\" value=\" " + antName[0] + "\" name=\"ant1\"><br><br>";
  tempStr += "<label for=\"ant2\">ANT-2: </label><input type=\"text\" id=\"ant2\" value=\" " + antName[1] + "\" name=\"ant2\"><br><br>";
  tempStr += "<label for=\"ant3\">ANT-3: </label><input type=\"text\" id=\"ant3\" value=\" " + antName[2] + "\"  name=\"ant3\"><br><br>";
  tempStr += "<label for=\"ant4\">ANT-4: </label><input type=\"text\" id=\"ant4\" value=\" " + antName[3] + "\"  name=\"ant4\"><br><br>";
  tempStr += "<label for=\"ant5\">ANT-5: </label><input type=\"text\" id=\"ant5\" value=\" " + antName[4] + "\"  name=\"ant5\"><br><br>";
  tempStr += NAMESendhtml;
  server.send(200, "text/html", tempStr);
}

//
//  Set Frequencies of antenna descriptors
//
void FREQchange()
{
  String tempStr;

  if (server.hasArg("ant1") || server.hasArg("ant2") ||
        server.hasArg("ant3") || server.hasArg("ant4") ||
          server.hasArg("ant5"))
  {
      antFreq[0] = server.arg("ant1");    // new assignments to Freqs...
      antFreq[1] = server.arg("ant2");
      antFreq[2] = server.arg("ant3");
      antFreq[3] = server.arg("ant4");
      antFreq[4] = server.arg("ant5");

      for(int x=0; x<5; x++)
      {
        strcpy(antennaFreq[x], antFreq[x].c_str()); // prep to store in EEPROM
        trim(antennaFreq[x]);
        antFreq[x] = (String)antennaFreq[x];
      }
      saveSettings();
  }
  tempStr = FREQSformhtml;
  tempStr += "<label for=\"ant1\">ANT-1: </label><input type=\"text\" id=\"ant1\" value=\" " + antFreq[0] + "\" name=\"ant1\"><br><br>";
  tempStr += "<label for=\"ant2\">ANT-2: </label><input type=\"text\" id=\"ant2\" value=\" " + antFreq[1] + "\" name=\"ant2\"><br><br>";
  tempStr += "<label for=\"ant3\">ANT-3: </label><input type=\"text\" id=\"ant3\" value=\" " + antFreq[2] + "\"  name=\"ant3\"><br><br>";
  tempStr += "<label for=\"ant4\">ANT-4: </label><input type=\"text\" id=\"ant4\" value=\" " + antFreq[3] + "\"  name=\"ant4\"><br><br>";
  tempStr += "<label for=\"ant5\">ANT-5: </label><input type=\"text\" id=\"ant5\" value=\" " + antFreq[4] + "\"  name=\"ant5\"><br><br>";
  tempStr += FREQSendhtml;
  server.send(200, "text/html", tempStr);
}

//
// base website served up...
//
void handleRoot() {
  
  String output;

  digitalWrite(led, 1);
  
  output = AntennaSiteHeader;
  
  // output the stat of each antenna selector 
    output += ButtonHeader;
    if (digitalRead(output1))   // reflect output state graphically
    {
      output += "active_state";
    }
    else
    {
      output += "inactive_state";
    }
    output += "\" href=\"ONE\""; 
    output += ButtonBody;
    output += antName[0];
    output += ButtonFooter;

  // output the stat of each antenna selector 
    output += ButtonHeader;
    if (digitalRead(output2)) 
    {
      output += "active_state";
    }
    else
    {
      output += "inactive_state";
    }
    output += "\" href=\"TWO\""; 
    output += ButtonBody;
    output += antName[1];
    output += ButtonFooter;

  // output the stat of each antenna selector 
    output += ButtonHeader;
    if (digitalRead(output3)) 
    {
      output += "active_state";
    }
    else
    {
      output += "inactive_state";
    }
    output += "\" href=\"THREE\""; 
    output += ButtonBody;
    output += antName[2];
    output += ButtonFooter;

  // output the stat of each antenna selector 
    output += ButtonHeader;
    if (digitalRead(output4)) 
    {
      output += "active_state";
    }
    else
    {
      output += "inactive_state";
    }
    output += "\" href=\"FOUR\""; 
    output += ButtonBody;
    output += antName[3];
    output += ButtonFooter;

  // output the stat of each antenna selector 
    output += ButtonHeader;
    if (digitalRead(output5)) 
    {
      output += "active_state";
    }
    else
    {
      output += "inactive_state";
    }
    output += "\" href=\"FIVE\""; 
    output += ButtonBody;
    output += antName[4];
    output += ButtonFooter;

  output += AntennaFooter;

  server.send(200, "text/html", output);
  
  digitalWrite(led, 0);
}


//
//  Change output states from button post
//
void Output_Change(int i)
{ 
  String output;

  digitalWrite(led, 1);
  
  output = AntennaSiteHeader;
  
  // output the stat of each antenna selector 
    output += ButtonHeader;

    digitalWrite(output1,LOW);
    digitalWrite(output2,LOW);
    digitalWrite(output3,LOW);
    digitalWrite(output4,LOW);
    digitalWrite(output5,LOW);        // Change over antenna output relays... BREAK B4 MAKE
    
    switch (i)  
    {
        case 1:
          digitalWrite(output1,HIGH);
          DEBUG_PORT.println("Antenna 1..");
          break;

        case 2:
          digitalWrite(output2,HIGH);
          DEBUG_PORT.println("Antenna 2..");
          break;

        case 3:
          digitalWrite(output3,HIGH);
          DEBUG_PORT.println("Antenna 3..");
          break;

        case 4:
          digitalWrite(output4,HIGH);
          DEBUG_PORT.println("Antenna 4..");
          break;

        case 5:
          digitalWrite(output5,HIGH);
          DEBUG_PORT.println("Antenna 5..");
          break;
    }
      
    handleRoot();

}

void Output_Change_ONE()
{
  Output_Change(1);
}
void Output_Change_TWO()
{
  Output_Change(2);
}
void Output_Change_THREE()
{
  Output_Change(3);
}
void Output_Change_FOUR()
{
  Output_Change(4);
}
void Output_Change_FIVE()
{
  Output_Change(5);
}

//
// Change host network paramters
//
void SSIDchange()
{
  digitalWrite(led, 1);

  String ssidstr;
  String pswdstr;

  if (server.hasArg("ssid") && server.hasArg("pswd")) 
  {
    ssidstr = server.arg("ssid");
    pswdstr = server.arg("pswd");
    int t1=ssidstr.length();
    int t2=pswdstr.length();

    String content = "<html><body><meta http-equiv=\"refresh\" content=\"0; URL='http://";
    content += WiFi.localIP().toString();
    content += "'\" /></body></html>";
    server.send(200, "text/html", content); 
              
    if ((t1 > 4) && (t2 > 7)) 
    {
        DEBUG_PORT.println("Resetting..");
        WiFi.persistent(false);
        WiFi.disconnect(true,true);
        delay(500);
        WiFi.mode(WIFI_OFF);
        delay(500);
        //station set up as station
        WiFi.mode(WIFI_STA);
        DEBUG_PORT.print("Connecting to..."); // use a LAN if there is one..

        strcpy(ssid, ssidstr.c_str());
        trim(ssid);
        strcpy(password, pswdstr.c_str());
        trim(password);

        DEBUG_PORT.print("[");
        DEBUG_PORT.print(ssid);        
        DEBUG_PORT.print("]-[");
        DEBUG_PORT.print(password);
        DEBUG_PORT.println("]");
        saveSettings();       // store setting in EEPROM for reboot..

        WiFi.begin(ssid, password);

        // Wait for connection
        int timeout = 0;  
        while (WiFi.status() != WL_CONNECTED) 
        {
          delay(500);
          DEBUG_PORT.print(".");    
          if(timeout++ > 10)
          {
              DEBUG_PORT.println("No LAN.. or wrong login");
              ESP.restart();      // avoid hopelessness and start over..
          }          
        }

        DEBUG_PORT.print("Connected to LAN: ");
        DEBUG_PORT.println(WiFi.SSID());
        DEBUG_PORT.print("New IP address: ");
        DEBUG_PORT.println(WiFi.localIP());

        // HOST_PORT.print("IP:");             // Notify host
        // HOST_PORT.println(WiFi.localIP());
        sprintf(myIPstr,"%s",WiFi.localIP().toString().c_str());
        return;
    }
    
    if(t2 < 8)
      DEBUG_PORT.print("Password too short - ");
    
    if(t1 < 4)
      DEBUG_PORT.print("SSID too short - ");

    DEBUG_PORT.println("Log in Failed");
  }

  server.send(200, "text/html", SSIDformhtml);
}


//
// 404 handler for url requests that go nowhere..
//
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "404 Site Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

//
// Initialize and setup page responders and LAN (if available)
//
void setup(void) {
 // uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG); //save WatchDog register
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  pinMode(input5, INPUT);
  
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output5, OUTPUT);

  digitalWrite(output1, digitalRead(input1));  // set default to front panel setting..
  digitalWrite(output2, digitalRead(input2));
  digitalWrite(output3, digitalRead(input3));
  digitalWrite(output4, digitalRead(input4));
  digitalWrite(output5, digitalRead(input5));

  DEBUG_PORT.begin(115200);   // programming and diag interface
  // HOST_PORT.begin(115200, SERIAL_8N1, RXD2_pin, TXD2_pin);  // interface to the main controller
  RADIO_PORT.begin(38400);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);


  EEPROM.begin(EEPROM_SIZE);
  tag[0] = EEPROM.read(TAG_ADDRESS);
  tag[1] = EEPROM.read(TAG_ADDRESS+1);

  if((tag[0] == TAG_INIT_0) && (tag[1] == TAG_INIT_1))
  { // initialized so get saved network information
      EERecover(SSID_ADDRESS, ssid);
      EERecover(PSWD_ADDRESS, password);
      DEBUG_PORT.println("SSID/Pass from EE");
      getNames();      // recover button names...
      getFreqs();      // recover frequency lists
      setupBandPlan(); // setup freq range list to select switch from
  }
  else
  {
    DEBUG_PORT.println("EEPROM not Initialize");
    strcpy(antennaName[0], "ANT-1");    // null out names to start
    strcpy(antennaName[1], "ANT-2");    // null out names to start
    strcpy(antennaName[2], "ANT-3");    // null out names to start
    strcpy(antennaName[3], "ANT-4");    // null out names to start
    strcpy(antennaName[4], "ANT-5");    // null out names to start
    strcpy(antennaName[5], "ANT-6");    // null out names to start

    strcpy(antennaFreq[0], "1");    // null out freqs to start
    strcpy(antennaFreq[1], "2");    // null out freqs to start
    strcpy(antennaFreq[2], "3");    // null out freqs to start
    strcpy(antennaFreq[3], "4");    // null out freqs to start
    strcpy(antennaFreq[4], "5");    // null out freqs to start
    strcpy(antennaFreq[5], "6");    // null out freqs to start

    strcpy(ssid,"HotBox2G");    // development fall back connections..
    strcpy(password,"INVALIDXX");
    saveSettings();
  }

  //station part
  WiFi.mode(WIFI_STA);
  DEBUG_PORT.print("connecting to..."); // use a LAN if there is one..
  DEBUG_PORT.println(ssid);
  WiFi.begin(ssid,password);

  randomSeed(analogRead(openAnalogPin));  // create temporary non-conflicting SSID for AP mode
  int timeout = 0;
  char tempAPssid[24];
  sprintf(tempAPssid,"WR9Ras%3d", (int)random(0,999));
  DEBUG_PORT.println(tempAPssid);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DEBUG_PORT.print(".");
    if(timeout++ > 10)
    {
        WiFi.disconnect();
        DEBUG_PORT.println("No LAN detected..");
        delay(2000);
        //access point part
        WiFi.mode(WIFI_AP);
        DEBUG_PORT.println("Creating Accesspoint");
        delay(500);
        WiFi.softAP(tempAPssid,"12345678",7,0,5);
        DEBUG_PORT.print("IP address:\t");
        DEBUG_PORT.println(WiFi.softAPIP());

        // HOST_PORT.print("IP:");             // Notify host
        // HOST_PORT.println(WiFi.softAPIP());
        sprintf(myIPstr,"%s",WiFi.softAPIP().toString().c_str());
        break;
    }
  }
  if(WiFi.status() == WL_CONNECTED)
  {
      DEBUG_PORT.println("");
      DEBUG_PORT.println("WiFi connected");
      DEBUG_PORT.println("IP address: ");
      DEBUG_PORT.println(WiFi.localIP());   

      // HOST_PORT.print("IP:");             // Notify host
      // HOST_PORT.println(WiFi.localIP());
      sprintf(myIPstr,"%s",WiFi.localIP().toString().c_str());
  }

  WiFi.setTxPower(WIFI_POWER_7dBm); 

  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp); //enable brownout detector

  if (MDNS.begin("WR9R Antenna Switch")) {
    DEBUG_PORT.println("MDNS responder started");
  }

  server.on("/", handleRoot);           // specify site addresses..
  server.on("/SSID", SSIDchange);
  server.on("/NAMES", NAMEchange);      // names for switch positions
  server.on("/FREQS", FREQchange);      // names for switch positions

  server.on("/ONE", Output_Change_ONE);
  server.on("/TWO", Output_Change_TWO);
  server.on("/THREE", Output_Change_THREE);
  server.on("/FOUR", Output_Change_FOUR);
  server.on("/FIVE", Output_Change_FIVE);

  server.onNotFound(handleNotFound);    // or 404...

  server.begin();
  DEBUG_PORT.println("HTTP server started");

// HOST_PORT.println("ok:");
  DEBUG_PORT.printf(myIPstr);
}

//
// Check and deal with handlers
//
void loop(void) {

  server.handleClient();

  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    switchStatus = digitalRead(input1);       // read front panel switch state
    switchStatus<<=1;
    switchStatus |= digitalRead(input2);
    switchStatus<<=1;
    switchStatus |= digitalRead(input3);
    switchStatus<<=1;
    switchStatus |= digitalRead(input4);
    switchStatus<<=1;
    switchStatus |= digitalRead(input5);

    if (lastSwitchStatus != switchStatus)
    {
      handleRoot();                           // if front panel changes update page...
      lastSwitchStatus = switchStatus;

      digitalWrite(output1, digitalRead(input1));  // then set outputs to front panel setting..
      digitalWrite(output2, digitalRead(input2));
      digitalWrite(output3, digitalRead(input3));
      digitalWrite(output4, digitalRead(input4));
      digitalWrite(output5, digitalRead(input5));      

      DEBUG_PORT.print("Panel Change - ");
      DEBUG_PORT.println(switchStatus);
    }

    read_Radio();   // check for comms from radio to see if a band change has happened
  }
 
}


