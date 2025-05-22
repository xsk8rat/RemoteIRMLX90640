#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
 #elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
 #endif
  
  #include <InfluxDbClient.h>
  #include <InfluxDbCloud.h>
  
  // WiFi AP SSID
  #define WIFI_SSID SECRET_SSID
  // WiFi password
  #define WIFI_PASSWORD SECRET_PASS
  
  #define INFLUXDB_URL    "http://192.168.68.79:8086"
  #define INFLUXDB_TOKEN  "REPLACEME"
  #define INFLUXDB_ORG    "REPLACEMETOO"
  #define INFLUXDB_BUCKET "AndAlsoREPLACEME"
  
  // Time zone info
  #define TZ_INFO "UTC-6"
  
