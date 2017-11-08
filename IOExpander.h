#ifndef __IOEXPANDER_H__
#define __IOEXPANDER_H__
#include <Arduino.h>
#include <Wire.h>

//device address
#define expander_addr 0x20

// iic io expander type
//__IOE_PCF8574__
//__IOE_MPC23017__
#define __IOE_PCF8574__

class IOExpander {
public:
	void begin();
	void digitalWrite(uint8_t pin,uint8_t value);
	uint8_t digitalRead(uint8_t pin);
	void writeByte(uint8_t data);
	uint8_t readByte();
private:
	uint8_t buf;
};

#endif