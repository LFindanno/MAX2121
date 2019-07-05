// MAX2121 Arduino Library
// Luigi Findanno 2019

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "max2121.h"

MAX2121::MAX2121(){
}

void MAX2121::begin(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
  init();
}

void MAX2121::init(){
	registers[N_DIVIDER_MSB] = 0b1000000;
	registers[N_DIVIDER_LSB] = 0b0000000;
	registers[CHARGE_PUMP] = 0b0001000;
	registers[F_DIVIDER_MSB] = 0b0000000;
	registers[F_DIVIDER_LSB] = 0b0000000;
	registers[XTAL_REF] = 0b00000001;
	registers[PLL] = 0b01000000;
	registers[VCO] = 0b11001100;
	registers[LP_FILTER] = 0b10010111;
	registers[CONTROL] = 0b0000000;
	registers[SHUTDOWN] = 0b0000000;
	registers[TEST] = 0b00001000;
	
	setLOfreq(1000000000);

}

void MAX2121::setLOfreq(uint32_t freq){
	uint16_t N;
	double q;
	uint32_t F;
	
	N = freq / MAX_CLK;
	registers[N_DIVIDER_MSB] = (N >> 8) | 0b10000000;
	registers[N_DIVIDER_LSB] = N & 0xFF;
	
	q = ((double)freq / MAX_CLK - (double)N) * 1048576;
	F = (uint32_t)q;
	registers[CHARGE_PUMP] = (registers[CHARGE_PUMP] & 0xF0) | (F >> 16) & 0x0F;
	registers[F_DIVIDER_MSB] = (F >> 8) & 0xFF;
	registers[F_DIVIDER_LSB] = F & 0xFF;
	
	if (freq >= 1125000000){
		registers[PLL] = registers[PLL] & 0b01111111;
	}else{
		registers[PLL] = registers[PLL] | 0b10000000;
	}
	update();
}

void MAX2121::setIFgain(uint8_t gain){
	
	if (gain > 15) gain = 15;
	registers[9] = (registers[9] & 0b11110000) | gain;
	update();
}

void standby(){
	registers[9] = registers[9] | 0b10000000;
	update();
}

void operate(){
	registers[9] = registers[9] & 0b01111111;
	update();

}

void MAX2121::update(){
	
#ifdef TWBR
	uint8_t twbrback = TWBR;
	TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz
#endif

	Wire.beginTransmission(_i2caddr);
	
	Wire.write(0); // write first register address = 0
	
	for(int i = 0; i < 12; i++){
		Wire.write(registers[i]);
	}
	
	Wire.endTransmission();
}

void MAX2121::readStatus(){
	int status1, status2;
	Wire.requestFrom(_i2caddr, 2);
	status1 = Wire.read();
	status2 = Wire.read();
	POR = (status1 & 0b10000000) >> 7;
	VASA = (status1 & 0b01000000) >> 6;
	VASE = (status1 & 0b00100000) >> 5;
	LD = (status1 & 0b00010000) >> 4;
	VCOSBR = (status2 & 0b11111000) >> 3;
	ADC = status2 & 0b00000111;	
}