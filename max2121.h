// MAX2121 Arduino Library
// Luigi Findanno 2019
// Default I2C Address 0x60

//#ifndef _MAX2121_H_
//#define _MAX2121_H_


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#include <Wire.h>

// MAX2121 quarz value
#define MAX_CLK 27000000

#define MAX_LO_MAX 2175000000UL
#define MAX_LO_MIN 925000000UL

// registers addresses
#define N_DIVIDER_MSB 0x00
#define N_DIVIDER_LSB 0x01
#define CHARGE_PUMP 0x02
#define F_DIVIDER_MSB 0x03
#define F_DIVIDER_LSB 0x04
#define XTAL_REF 0x05
#define PLL 0x06
#define VCO 0x07
#define LP_FILTER 0x08
#define CONTROL 0x09
#define SHUTDOWN 0X0A
#define TEST 0x0B
#define STATUS_1 0x0C
#define STATUS_2 0x0D



class MAX2121{
 public:
  MAX2121();
  void begin(uint8_t a); // param i2c address (default 0x60)
  void setIFgain(uint8_t gain); // output gain, range 0 - 15 (db)
  void setLOfreq(uint32_t freq); // min 925000000, max 2175000000
  void readStatus(); // call this before read the following variables
  void standby(); // set the chip in standby mode
  void operate(); // set the chip in normal operation mode
  int POR, VASA, VASE, LD, VCOSBR, VASADC;

 private:
  uint8_t registers[14];
  uint8_t _i2caddr;
  void init();
  void update();
};