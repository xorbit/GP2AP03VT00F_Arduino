// Sharp GP2AP03VT00F Time-Of-Flight sensor driver example
//
// Copyright (c) 2024 Silicognition LLC
//
// Licensed under MIT License

#include "GP2AP03VT00F.h"


// Create the distance sensor object
GP2AP03VT00F distSense = GP2AP03VT00F();

// Setup code
void setup() {
  // Initialize serial printing
  Serial.begin(115200);
  Serial.println("GP2AP03VT00F Time-Of-Flight sensor polling example");
  
  // Initialize the I2C bus before the sensor, the sensor needs it!
  Wire.begin();

  // Initialize distance sensor object
  if (distSense.begin()) {  
    // Init was successful, start first measurement
    distSense.startMeasurement();
  } else {
    // Failed to init
    Serial.println("Failed to initialize GP2AP03VT00F sensor!");
    // Don't run the main loop
    for (;;)
      delay(1000);
  }
}

// Main loop code
void loop() {

  // Check if distance sensor measurement finished
  if (distSense.checkMeasurementEnd()) {
    // Measurement finished, print results formatted for use with Serial plotter
    Serial.print("coarse:");
    Serial.print(distSense.readCoarseDistance());
    Serial.print(",fine:");
    Serial.print(distSense.readFineDistance());
    Serial.println();
    
    // Start next measurement
    distSense.startMeasurement();
  }
  
  // The above is non-blocking, execute other application code here
  // But if you want to make sure the distance sensor gets serviced regularly,
  // this other code should be non-blocking as well
}

