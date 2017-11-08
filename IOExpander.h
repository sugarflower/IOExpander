#ifndef __IOEXPANDER_H__
#define __IOEXPANDER_H__
#include <Arduino.h>
#include <Wire.h>

// iic io expander type
//__IOE_PCF8574__
//__IOE_MPC23017__
#define __IOE_PCF8574__

class IOExpander {
public:
	void begin();
	void beginSPI();
	void setDeviceAddress(uint8_t addr);
	void setSPIAddress(uint8_t addr);
	void digitalWrite(uint8_t pin,uint8_t value);
	uint8_t digitalRead(uint8_t pin);
	void writeSPI(uint16_t addr,uint8_t value);
	uint8_t readSPI(uint16_t addr);
	void writeByte(uint8_t data);
	uint8_t readByte();
	void pinMode(uint8_t pin,uint8_t mode);
private:
	uint8_t spiBuf[4];
	uint8_t device_addr;
	uint8_t buf;
	uint8_t spi_addr;
	uint8_t CS;
	uint8_t SCK;
	uint8_t SI;
	uint8_t SO;
};

#endif
