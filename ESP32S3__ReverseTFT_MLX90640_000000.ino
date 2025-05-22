
/*
 ESP323S3 -> Hold D0 and press RESET

000 Adding MLX90640 Sensor 
*/

#include "arduino_secrets.h" 
#include "InfluxDefs.h" 

#include <SPI.h>
#include <Wire.h>

#include <TimeLib.h>
/* Time Global Variables
*/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -6*3600;
const int   daylightOffset_sec = 3600;

char CODENAME[] = __FILE__;
char DEVNAME[]  = "InfluxReverseTFT_020";
char LOCNAME[]  = "Kira";


  // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  // Declare Data point
  Point sensor("Environment_Study");


// = = = = = = 
#include <Adafruit_HDC302x.h>
Adafruit_HDC302x hdc = Adafruit_HDC302x();

// = = = = = = 
#include <SoftTimers.h>
SoftTimer serial_timer; 
SoftTimer sampleTimer;
SoftTimer displayTimer;
bool isFirstPass = true;

// = = = = = = 
//Display Stuff 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// Use dedicated hardware SPI pins
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// = = = = = = 
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640

#define TA_SHIFT 8 //Default shift for MLX90640 in open air

static float mlx90640To[768];
int iLengthArray = 768;
paramsMLX90640 mlx90640;

float maxTempF = 0.0;
float minTempF = 0.0;
float meanTempF = 0.0;

int IRtempreads  = 0;

// = = = = = = ==============================================================================
//             setup ()
// = = = = = = ==============================================================================
void setup() {

  Wire.begin();
  Wire.setClock(400000); //Increase I2C clock speed to 400kHz

  Serial.begin(115200);
  Serial.println(trimFileName(CODENAME));

  ConnectToWiFiInFlux();  

  initializeHDC3022();

  initializeDISPLAYST77XX();

  initAndSetTime();

  //Set timer delays in seconds
  timerResetSeconds(serial_timer, 30.0);
  timerResetSeconds(sampleTimer,  60.0);
  timerResetSeconds(displayTimer, 10.0);

  initMLX90640();
  Serial.println(trimFileName(CODENAME));
}
// = = = = = = ==============================================================================
//             loop ()
// = = = = = = ==============================================================================
void loop() {

  if (sampleTimer.hasTimedOut() || isFirstPass) {
    readMLX90640();
    maxTempF =  (9.0/5.0)*GetArrayMax(mlx90640To,iLengthArray)+32.0;
    minTempF =  (9.0/5.0)*GetArrayMin(mlx90640To,iLengthArray)+32.0;
    meanTempF = (9.0/5.0)*GetArrayMean(mlx90640To,iLengthArray)+32.0;
    IRtempreads++;
    Serial.print("IRtempreads = "); 
    Serial.println(IRtempreads);

    SubmitDataToInflux();
    sampleTimer.reset();
  }
  // Print to Serial line
  if (serial_timer.hasTimedOut() || isFirstPass) {
    serial_timer.reset();
    printHDC3022Report();
    printWifiStatus();
  } 
  // Print to DISPLAY
  if (displayTimer.hasTimedOut() || isFirstPass) {
    readMLX90640();
    maxTempF =  (9.0/5.0)*GetArrayMax(mlx90640To,iLengthArray)+32.0;
    minTempF =  (9.0/5.0)*GetArrayMin(mlx90640To,iLengthArray)+32.0;
    meanTempF = (9.0/5.0)*GetArrayMean(mlx90640To,iLengthArray)+32.0;
    IRtempreads++;
    Serial.print("IRtempreads = "); 
    Serial.println(IRtempreads);

    displayTimer.reset();
    displayValues();
  } 
  
  isFirstPass = false;
  delay(1000);
}



// = = = = = = ==============================================================================
//             timerReset
// Sets the timers usingf the same code,
void timerResetSeconds(SoftTimer &inTimer, float inSec) {
  inTimer.setTimeOutTime(1);
  inTimer.reset();
  delay(2);
  inTimer.setTimeOutTime(inSec*1000);
  inTimer.reset();
}
// = = = = = = ==============================================================================
//             trimFileName ()
// = = = = = = ==============================================================================
String trimFileName(char inStr[]) {
  String myFile = String(inStr);
  int      useI = myFile.lastIndexOf("\\");
  if (useI<1) {
    return("");
  }
  //Serial.print("useI  = ");Serial.println(useI);
  myFile = myFile.substring(useI+1);
  //Serial.print("myFile = ");Serial.println(myFile);
  return(myFile);
}

// = = = = = = ==============================================================================
//             IpAddress2String ()
//             https://forum.arduino.cc/t/how-to-manipulate-ipaddress-variables-convert-to-string/222693/5
// = = = = = = ==============================================================================
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}


// = = = = = = ==============================================================================
//              getIPAddressString ()
//              Abstracting, as it may be different for different systems.
// = = = = = = ==============================================================================
String getIPAddressString() {
  IPAddress ip = WiFi.localIP();
  return IpAddress2String(ip);
}
// = = = = = = ==============================================================================
//             printWifiStatus ()
// = = = = = = ==============================================================================
void printWifiStatus() {
  // print the SSID of the network to which you're attached:
  Serial.print("Connected to SSID      -> ");
  Serial.println(WiFi.SSID());
  // Print the IP address of the device
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address             -> ");
  Serial.println(ip);
  byte mac[6]; 
  WiFi.macAddress(mac);
  Serial.print("MAC Address            -> ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[0],HEX);
  Serial.println();
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal Strength (RSSI) -> ");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.println("");
}


