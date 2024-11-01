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

#ifndef _GP2AP03VT00F_H_
#define _GP2AP03VT00F_H_

#include <Wire.h>

// Range status flags
#define RANGE_STATUS_VALID_DATA           0x00
#define RANGE_STATUS_VCSEL_SHORT          0x01
#define RANGE_STATUS_LOW_SIGNAL           0x02
#define RANGE_STATUS_LOW_SN               0x04
#define RANGE_STATUS_TOO_MUCH_AMB         0x08
#define RANGE_STATUS_WAF                  0x10
#define RANGE_STATUS_CAL_ERROR            0x20  
#define RANGE_STATUS_CROSSTALK_ERROR      0x80


// TOF sensor class interface
class GP2AP03VT00F {
  public:
    GP2AP03VT00F(void);
    int begin(int8_t en = -1, TwoWire *pwire = &Wire);
    void end(void);
    void startMeasurement(void);
    void shutdown(void);
    int checkMeasurementEnd(void);
    void clearMeasurementEnd(void);
    uint8_t readCoarseDistance(void);
    uint8_t readFineDistance(void);
    uint8_t readRangeStatus(void);

  private:
    TwoWire *_pwire;
    int8_t _en;

    void readi2c(uint8_t regAddr, uint8_t* data, int len);
    void writei2c(uint8_t regAddr, uint8_t* data, int len);
    uint8_t getRegister(uint8_t regAddr);
    void setRegister(uint8_t regAddr, uint8_t value);
};

#endif
