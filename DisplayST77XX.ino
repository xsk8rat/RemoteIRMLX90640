/*
https://learn.adafruit.com/esp32-s3-reverse-tft-feather#arduino-ide-setup-2
https://learn.adafruit.com/adafruit-gfx-graphics-library/
https://github.com/adafruit/Adafruit-ST7735-Library
*/
// = = = = = = ==============================================================================
//             initializeDISPLAY
// Commands to start and configure the display on the ESP32S2 Reverse TFT.
void initializeDISPLAYST77XX() {
    // DISPLAY turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  Serial.println("Display Started:");
}

void displayValues() {
  int yStart = 5;
  tft.setRotation(3);

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(3);
  tft.setCursor(10, yStart);
  tft.print(getTemperatureF());
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("F");

  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(3);
  tft.setCursor(10, yStart+25);
  tft.print(getRelHumidity());
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("%RH");
  
  int xHours   = getHours();
  int xMinutes = getMinutes();

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(4);
  tft.setCursor(10, yStart+55);
  if (xHours<10) tft.print(0); 
  tft.print(xHours);
  tft.print(":");
  if (xMinutes<10) tft.print(0); 
  tft.print(xMinutes);


  int xStartTemp = 145;
  // maxTempF
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(xStartTemp, yStart);
  tft.setTextWrap(false);
  tft.print(maxTempF,1);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Fmax");

  // minTempF
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(xStartTemp, yStart + 25);
  tft.setTextWrap(false);
  tft.print(minTempF,1);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Fmin");

  // meanTempF
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(xStartTemp, yStart + 50);
  tft.setTextWrap(false);
  tft.print(meanTempF,1);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Fave");

  long rssi = WiFi.RSSI();
  tft.setTextColor(ST77XX_ORANGE);
  tft.setTextSize(2);
  tft.setCursor(180, yStart+70);
  tft.setTextWrap(false);
  tft.print(rssi);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("db");
  
  tft.setTextColor(ST77XX_ORANGE);
  tft.setTextSize(2);
  tft.setCursor(0, 95);
  tft.setTextWrap(false);
  tft.println(getIPAddressString());

  tft.setTextColor(ST77XX_ORANGE);
  tft.setTextSize(2);
  tft.setCursor(0, 117);
  tft.setTextWrap(false);
  tft.print(DEVNAME);

}