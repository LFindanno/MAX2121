// Luigi Findanno 2019
// Convertino L Band Downconverter board example program
// https://luigifindanno.home.blog/convertino-downconverter/
// It simply sets the frequency of the Local Oscillator to 1175 MHz
// Local Oscillator Frequency range: 925 to 2175 Mhz

#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "max2121.h"

Adafruit_MCP4725 rfgain; // DAC to set the RF gain using pin 5 MAX2121
MAX2121 tuner;

void setup() {
 Serial.begin(9600);

 rfgain.begin(0x61);  // I2C address of MCP4725
 rfgain.setVoltage(621, false); // 620 = 0.5V = max gain , 3375 = 2.7V = min gain
 
 tuner.begin(0x60); // I2C address of MAX2121
 delay(100);
 
 tuner.setLOfreq(1175000000); // set the local oscillator frequency
 delay(1000);
 
 tuner.readStatus(); // read the status of MAX2121 and print to the serial port
 Serial.print("Power on Reset: ");
 Serial.println(tuner.POR);
 Serial.print("VASA: ");
 Serial.println(tuner.VASA);
 Serial.print("VASE: ");
 Serial.println(tuner.VASE);
 Serial.print("PLL Locked: ");
 Serial.println(tuner.LD);
 Serial.print("VCO band: ");
 Serial.println(tuner.VCOSBR);
 Serial.print("VAS ADC: ");
 Serial.println(tuner.VASADC);

}

void loop() {

}
