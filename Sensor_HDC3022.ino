// = = = = = = ==============================================================================
//             initializeHDC3022 ()
// = = = = = = ==============================================================================
void initializeHDC3022() {
  Serial.println("HDC3022 Start Initialization.");  
  if (! hdc.begin(0x44, &Wire)) {
    Serial.println("Could not find HDC3022 sensor?");
    while (1);
  }
  Serial.println("HDC3022 Initialized.");  
}

// = = = = = = ==============================================================================
//             printHDC3022Report ()
// = = = = = = ==============================================================================
void printHDC3022Report() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);

  Serial.print("Temperature  = ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Temperature  = ");
  Serial.print(temp*(9./5.)+32.);
  Serial.println(" °F");

  Serial.print("Humidity     = ");
  Serial.print(RH);
  Serial.println(" %");
}


// = = = = = = ==============================================================================
//             getTemperatureC ()
// = = = = = = ==============================================================================
float getTemperatureC() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);
  return( float(temp));
}
// = = = = = = ==============================================================================
//             getTemperatureF ()
// = = = = = = ==============================================================================
float getTemperatureF() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);
  return( 32.0+float(temp*(9./5.)));
}

// = = = = = = ==============================================================================
//             getRelHumidity ()
// = = = = = = ==============================================================================
float getRelHumidity() {
  double temp = 0.0;
  double RH = 0.0;
  hdc.readTemperatureHumidityOnDemand(temp, RH, TRIGGERMODE_LP0);
  return( float(RH));
}