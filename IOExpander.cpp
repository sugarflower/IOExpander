#include "IOExpander.h"

void IOExpander::begin(){
	Wire.begin();
}

void IOExpander::setDeviceAddress(uint8_t addr){
	IOExpander::device_addr=addr;
}

void IOExpander::digitalWrite(uint8_t pin,uint8_t value){
	IOExpander::buf=IOExpander::readByte();
	if( value == 1 ){
		IOExpander::buf |= ( 1 << pin ); // set bit
	} else {
		IOExpander::buf &= ~( 1 << pin ); // reset bit
	}
	IOExpander::writeByte(IOExpander::buf);
}

uint8_t IOExpander::digitalRead(uint8_t pin){
	return (readByte()>>pin) & 1;
}

#ifdef __IOE_PCF8574__
uint16_t IOExpander::readByte(){
	Wire.requestFrom(device_addr,1);
	while( !Wire.available() ){
		return Wire.read();
	}
}

void IOExpander::writeByte(uint16_t data){
	Wire.beginTransmission(device_addr);
	Wire.write(data & 0xff);
	Wire.endTransmission();
}

// pcf8574 hasn't have pinmode this is dummy
void IOExpander::pinMode(uint8_t pin, uint8_t mode){}
#endif


#ifdef __IOE_MCP23017__
uint16_t IOExpander::readByte(){
	IOExpander::buf = 0;
	Wire.beginTransmission(device_addr);
	Wire.write(0x13);
	Wire.endTransmission();
	Wire.requestFrom(device_addr,1);
	if( Wire.available() ){
		IOExpander::buf |= Wire.read();
	}
	IOExpander::buf = IOExpander::buf << 8;
	Wire.beginTransmission(device_addr);
	Wire.write(0x12);
	Wire.endTransmission();
	Wire.requestFrom(device_addr,1);
	if( Wire.available() ){
		IOExpander::buf |= Wire.read();
	}
	return IOExpander::buf;
}

void IOExpander::writeByte(uint16_t data){
	Wire.beginTransmission(device_addr);
	Wire.write(0x14);
	Wire.write(data & 0xff);
	Wire.endTransmission();	

	Wire.beginTransmission(device_addr);
	Wire.write(0x15);
	Wire.write((data>>8) & 0xff);
	Wire.endTransmission();	
}

void IOExpander::pinMode(uint8_t pin, uint8_t mode){
	uint8_t port;
	if(pin > 7 ) {
		port=1;
	} else {
		port=0;
	}
	Wire.beginTransmission(device_addr);
	Wire.write(port);
	Wire.endTransmission();
	Wire.requestFrom(device_addr,1);
	while( !Wire.available() ){
		IOExpander::buf=Wire.read();
	}
	if(mode==OUTPUT){
		IOExpander::buf &= ~( 1 << pin ); // reset bit
	} else {
		IOExpander::buf |= ( 1 << pin ); // set bit
	}
	Wire.beginTransmission(device_addr);
	switch(mode){
		case INPUT:
		case OUTPUT:
			Wire.write(0x00 + port);
			Wire.write(IOExpander::buf);
			Wire.endTransmission();

			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.endTransmission();
			Wire.requestFrom(device_addr,1);
			while( !Wire.available() ){
				IOExpander::buf=Wire.read();
			}
			IOExpander::buf &= ~( 1 << pin ); // reset bit
			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.write(IOExpander::buf);
			break;
		case INPUT_PULLUP:
			Wire.write(0x00 + port);
			Wire.write(IOExpander::buf);
			Wire.endTransmission();

			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.endTransmission();
			Wire.requestFrom(device_addr,1);
			while( !Wire.available() ){
				IOExpander::buf=Wire.read();
			}
			IOExpander::buf |= ( 1 << pin ); // set bit
			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.write(IOExpander::buf);
			break;
	}
	Wire.endTransmission();
}
#endif




void IOExpander::beginSPI(){
	IOExpander::SCK=2;
	IOExpander::SI =1;
	IOExpander::SO =0;
	IOExpander::pinMode(SCK,OUTPUT);
	IOExpander::pinMode(SI,OUTPUT);
	IOExpander::pinMode(SO,INPUT);
}
void IOExpander::setSPIAddress(uint8_t addr){
	IOExpander::spi_addr=addr;
}
void IOExpander::spiWriteByte(uint8_t value){
	for(int i=0; i<8; i++){
		IOExpander::digitalWrite(IOExpander::SCK,LOW);
		IOExpander::digitalWrite(IOExpander::SI,(value&(0x80>>i))!=0 ? HIGH : LOW );
		IOExpander::digitalWrite(IOExpander::SCK,HIGH);
	}
}
uint8_t IOExpander::spiReadByte(){
	uint8_t ret=0;
	for(int i=0; i<8; i++){
		IOExpander::digitalWrite(IOExpander::SCK,LOW);
		ret |= IOExpander::digitalRead(IOExpander::SO)<<(7-i);
		IOExpander::digitalWrite(IOExpander::SCK,HIGH);
	}
	IOExpander::digitalWrite(IOExpander::CS,HIGH);
	return ret;
}
/*
Arduino.h

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
*/
