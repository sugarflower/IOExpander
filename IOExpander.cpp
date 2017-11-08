#include "IOExpander.h"

void IOExpander::begin(){
	Wire.begin();
}

void IOExpander::digitalWrite(uint8_t pin,uint8_t value){
	IOExpander::buf=IOExpander::readByte();
	if( value == 1 ){
		IOExpander::buf |= ( 1 << pin );
	} else {
		IOExpander::buf &= ~( 1 << pin );
	}
	IOExpander::writeByte(IOExpander::buf);
}

uint8_t IOExpander::digitalRead(uint8_t pin){
	return (readByte()>>pin) & 1;
}

#ifdef __IOE_PCF8574__
uint8_t IOExpander::readByte(){
	Wire.requestFrom(expander_addr,1);
	if( Wire.available() ){
		return Wire.read();
	}
}

void IOExpander::writeByte(uint8_t data){
	Wire.beginTransmission(expander_addr);
	Wire.write(data);
	Wire.endTransmission();
}
#endif
