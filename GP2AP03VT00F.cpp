// Sharp GP2AP03VT00F Time-Of-Flight sensor driver
//
// Based on work found here:
// https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/GP2AP02VT00F/sharp_gp2ap02vt00f_demo.ino
// Copyright (c) 2018 sharpsensoruser
//
// Modified and improved by Patrick Van Oosterwijck
// Copyright (c) 2024 Silicognition LLC
//
// Licensed under MIT License

#include "GP2AP03VT00F.h"


// Sensor I2C slave address (7-bit)
#define I2C_SLAVE_ADDRESS                 0x29

// Device/Chip ID
#define CHIP_ID                           0x2F

// Sensor registers
#define REGISTER_OPERATING_MODE           0x00
#define REGISTER_MEASUREMENT_STATUS       0x01
#define REGISTER_SOFTWARE_RESET           0x02
#define REGISTER_INT_TERMINAL             0x02
#define REGISTER_VCSEL_CURRENT            0x04
#define REGISTER_MEASUREMENT_TIME         0x2B
#define REGISTER_RANGE1_STATUS            0x2E
#define REGISTER_MEASUREMENT_DATA         0x38
#define REGISTER_DEVICE_ID                0x41

// Operating mode
#define OPERATING_MODE_SHUTDOWN           0
#define OPERATING_MODE_START_MEASUREMENT  0x80

// Measurement status
#define BITMASK_MEASUREMENT_STATUS_FLAG   0x02
#define BITMASK_MEASUREMENT_STATUS_CLEAR  0x0D

// Software reset
#define SOFTWARE_RESET_VALUE              0x01

// INT terminal
#define INT_TERMINAL_VALUE                0x34

// VCSEL current
#define VCSEL_CURRENT_SHORT_RANGE         0x07
#define VCSEL_CURRENT_LONG_RANGE          0x0F

// Measurement time
#define MEASUREMENT_TIME_NORMAL           0x03

// Register values
#define REGISTER_06H_BASE_VALUE           0x60
#define REGISTER_14H_SHORT_RANGE          0x28
#define REGISTER_14H_LONG_RANGE           0x14
#define REGISTER_43H_SHORT_RANGE          0xC0
#define REGISTER_43H_LONG_RANGE           0xD0
#define REGISTER_47H_SHORT_RANGE          0x20
#define REGISTER_47H_LONG_RANGE           0x00

// Measurement data addresses
#define MEASUREMENT_DATA_COARSE           100
#define MEASUREMENT_DATA_FINE             110


// Constructor
GP2AP03VT00F::GP2AP03VT00F(void) :
  _pwire(NULL), _en(-1)
{}

// Helper function to read from I2C
void GP2AP03VT00F::readi2c(uint8_t regAddr, uint8_t* data, int len) {
  if (!this->_pwire || !data) return;

  this->_pwire->beginTransmission(I2C_SLAVE_ADDRESS);
  this->_pwire->write(regAddr);
  this->_pwire->endTransmission();

  this->_pwire->requestFrom(I2C_SLAVE_ADDRESS, len);
  for(int i = 0; i < len; i++) {
    data[i] = this->_pwire->read();
  }
}

// Helper function to write to I2C
void GP2AP03VT00F::writei2c(uint8_t regAddr, uint8_t* data, int len) {
  if (!this->_pwire || !data) return;

  this->_pwire->beginTransmission(I2C_SLAVE_ADDRESS);
  this->_pwire->write(regAddr);
  for(int i = 0; i < len; i++) {
    this->_pwire->write(data[i]);
  }
  this->_pwire->endTransmission(); 
}

// Helper to get single register value
uint8_t GP2AP03VT00F::getRegister(uint8_t regAddr) {
  uint8_t data = 0;
  readi2c(regAddr, &data, 1);
  return data;
}

// Helper to set the value of a single register
void GP2AP03VT00F::setRegister(uint8_t regAddr, uint8_t value) {
  writei2c(regAddr, &value, 1);
}

// Begin method
int GP2AP03VT00F::begin(int8_t en, TwoWire *pwire) {
  // Save the parameters
  this->_pwire = pwire;
  this->_en = en;

  // If we were given an enable pin, configure it
  if (this->_en >= 0) {
    pinMode(this->_en, OUTPUT);
    digitalWrite(this->_en, HIGH);
  }

  // Were we given an I2C device?
  if (this->_pwire) {
    // Attempt to perform software reset
    setRegister(REGISTER_SOFTWARE_RESET, SOFTWARE_RESET_VALUE);
    setRegister(REGISTER_OPERATING_MODE, OPERATING_MODE_START_MEASUREMENT);
    delay(2);
    setRegister(REGISTER_OPERATING_MODE, OPERATING_MODE_SHUTDOWN);
    setRegister(REGISTER_RANGE1_STATUS, RANGE_STATUS_VALID_DATA);

    // Try to access the device to read the chip ID
    if (getRegister(REGISTER_DEVICE_ID) == CHIP_ID) {
      // We detected the correct chip, set it up
      setRegister(REGISTER_MEASUREMENT_TIME, MEASUREMENT_TIME_NORMAL);
      setRegister(REGISTER_INT_TERMINAL, INT_TERMINAL_VALUE);
      setRegister(0x03, 0x00);
      setRegister(REGISTER_VCSEL_CURRENT, VCSEL_CURRENT_LONG_RANGE);
      setRegister(0x05, 0x0C);
      setRegister(0x06, REGISTER_06H_BASE_VALUE | VCSEL_CURRENT_LONG_RANGE);
      setRegister(0x07, 0xDF);
      setRegister(0x0D, 0x82);
      setRegister(0x0E, 0x02);
      setRegister(0x11, 0xAD);
      setRegister(0x12, 0x00);
      setRegister(0x13, 0x15);
      setRegister(0x14, REGISTER_14H_LONG_RANGE);
      setRegister(0x15, 0x01);
      setRegister(0x1A, 0xAA);
      setRegister(0x1B, 0xAA);
      setRegister(0x1C, 0xAA);
      setRegister(0x1D, 0xAA);
      setRegister(0x1E, 0xF4);
      setRegister(0x1F, 0xF7);
      setRegister(0x28, 0x84);
      setRegister(0x2A, 0x0B);
      setRegister(0x43, REGISTER_43H_LONG_RANGE);
      setRegister(0x45, 0x04);
      setRegister(0x47, REGISTER_47H_LONG_RANGE);
      setRegister(0x4D, 0x06);
      setRegister(0x4E, 0x3E);

      // And report that we initialized the chip
      return 1;
    }
  }
  // We don't have an I2C device or didn't read the correct ID so fail
  return 0;
}

// End method
void GP2AP03VT00F::end(void) {
  // Set the sensor shutdown mode
  shutdown();

  // If we were given an enable pin, deconfigure it
  if (this->_en >= 0) {
    pinMode(this->_en, INPUT);
  }
}

// Set shutdown mode
void GP2AP03VT00F::shutdown(void) {
  setRegister(REGISTER_OPERATING_MODE, OPERATING_MODE_SHUTDOWN);
}

// Start a measurement
void GP2AP03VT00F::startMeasurement(void) {
  clearMeasurementEnd();
  setRegister(REGISTER_OPERATING_MODE, OPERATING_MODE_START_MEASUREMENT);
}

// Check if measurement has ended
int GP2AP03VT00F::checkMeasurementEnd(void) {
  return (getRegister(REGISTER_MEASUREMENT_STATUS) &
          BITMASK_MEASUREMENT_STATUS_FLAG) == BITMASK_MEASUREMENT_STATUS_FLAG;
}

// Clear measurement end flag
void GP2AP03VT00F::clearMeasurementEnd(void) {
  setRegister(REGISTER_MEASUREMENT_STATUS, BITMASK_MEASUREMENT_STATUS_CLEAR);
}

// Read coarse distance
uint8_t GP2AP03VT00F::readCoarseDistance(void) {
  uint8_t data[2];
  setRegister(REGISTER_MEASUREMENT_DATA, MEASUREMENT_DATA_COARSE);
  readi2c(REGISTER_MEASUREMENT_DATA, data, 2);
  return data[1];
}

// Read fine distance
uint8_t GP2AP03VT00F::readFineDistance(void) {
  uint8_t data[2];
  setRegister(REGISTER_MEASUREMENT_DATA, MEASUREMENT_DATA_FINE);
  readi2c(REGISTER_MEASUREMENT_DATA, data, 2);
  return data[1];
}

// Read range status
uint8_t GP2AP03VT00F::readRangeStatus(void) {
  return getRegister(REGISTER_RANGE1_STATUS);
}

