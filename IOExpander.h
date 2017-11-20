#ifndef __IOEXPANDER_H__
#define __IOEXPANDER_H__
#include <Arduino.h>
#include <Wire.h>

// iic io expander type
#define __IOE_MCP23017__
//#define __IOE_PCF8574__

class IOExpander {
public:
	void begin();
	void beginSPI(uint8_t sck,uint8_t si,uint8_t so);
	void setDeviceAddress(uint8_t addr);

	void digitalWrite(uint8_t pin,uint8_t value);
	uint8_t digitalRead(uint8_t pin);
	void spiWriteByte(uint8_t value);
	uint8_t spiReadByte();

	void writeByte(uint16_t data);
	uint16_t readByte();
	void pinMode(uint8_t pin,uint8_t mode);
private:
	uint8_t device_addr;
	uint16_t buf;

	uint8_t SCK;
	uint8_t SI;
	uint8_t SO;

	boolean spi_mode;
};

#endif
