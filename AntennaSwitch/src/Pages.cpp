/*
    site page information (HTML and JSON)

*/
#include "AS_Wifi.h"

String AntennaSiteHeader = "<!DOCTYPE html><html><body><link rel=\"icon\" href=\"data:,\">\
<head><title>WR9R Antenna Selector</title></head><body style=\"background-color:black;\">\
<meta http-equiv=\"refresh\" content=\"5; URL=/\"> <style>\
.inactive_state {color: #fff !important;text-transform: uppercase;text-decoration: none;background: #60a3bc;\
padding: 20px;border-radius: 50px;display: inline-block;border: none;transition: all 0.4s ease 0s;}\
.inactive_state:hover {text-shadow: 0px 0px 6px rgba(255, 255, 255, 1);-webkit-box-shadow: 0px 5px 40px -10px rgba(0,0,0,0.57);\
-moz-box-shadow: 0px 5px 40px -10px rgba(0,0,0,0.57);\transition: all 0.4s ease 0s;}\
.active_state {color: #000 !important;text-transform: uppercase;text-decoration: none;background: #07fc03;padding: 20px;\
border-radius: 50px;display: inline-block;border: none;transition: all 0.4s ease 0s;}\
.active_state:hover {text-shadow: 0px 0px 6px rgba(255, 255, 255, 1);-webkit-box-shadow: 0px 5px 40px -10px rgba(0,0,0,0.57);\
-moz-box-shadow: 0px 5px 40px -10px rgba(0,0,0,0.57);transition: all 0.4s ease 0s;}\
</style>";

String ButtonHeader = "<div class=\"button_cont\" align=\"center\"><a class=\"";            /* active inactive */
String ButtonBody = "target=\"_self\" rel=\"nofollow noopener\">";  /* Button name */
String ButtonFooter = "</a></div><br>";
String AntennaFooter = "</body></html>";

const char SSIDformhtml[] = "<!DOCTYPE html><html><body><link rel=\"icon\" href=\"data:,\">\
<head><title>WR9R Antenna Selector</title></head>\
<body> <h2>WR9R Antenna Selector</h2>\
<form method=\"get\" target=\"_self\"><label for=\"ssid\">SSID: </label>\
<input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br><label for=\"pswd\">PSWD: </label>\
<input type=\"text\" id=\"pswd\" name=\"pswd\"><br><br><input type=\"submit\" value=\"Submit\">\
</body></html></form>";


const char NAMESformhtml[] = "<!DOCTYPE html><html><body><link rel=\"icon\" href=\"data:,\">\
<head><title>WR9R Antenna Selector</title></head>\
<body> <h2>WR9R Antenna Selector</h2>\
<form method=\"get\" target=\"_self\">";

const char NAMESendhtml[] = "<input type=\"submit\" value=\"Submit\"></body></html></form>";


/*
 <label for=\"ant2\">ANT-2: </label><input type=\"text\" id=\"ant2\" name=\"ant2\"><br><br>
 <label for=\"ant3\">ANT-3: </label><input type=\"text\" id=\"ant3\" name=\"ant3\"><br><br>
 <label for=\"ant4\">ANT-4: </label><input type=\"text\" id=\"ant4\" name=\"ant4\"><br><br>
 <label for=\"ant5\">ANT-5: </label><input type=\"text\" id=\"ant5\" name=\"ant5\"><br><br>
*/