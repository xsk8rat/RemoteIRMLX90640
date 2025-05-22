// = = = = = = ==============================================================================
//             SubmitDataToInflux ()
// = = = = = = ==============================================================================
void SubmitDataToInflux() {
    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();
    sensor.addField("rssi",               WiFi.RSSI());
    sensor.addField("temperature",        getTemperatureF());
    sensor.addField("humidity",           getRelHumidity());

    if (IRtempreads >= 2) {
      sensor.addField("IRMaxTemp",           maxTempF);
      sensor.addField("IRMinTemp",           minTempF);
      sensor.addField("IRMeanTemp",          meanTempF);
    }


    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());
  
    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }
  
    // Write point
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    } else {
      Serial.println("Write Successful.");
    }


}

// = = = = = = ==============================================================================
//             ConnectToWiFiInFlux ()
// = = = = = = ==============================================================================
  void ConnectToWiFiInFlux() {
    // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    Serial.println();
  
    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  
  
    // Check server connection
    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());
    } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
    }

}