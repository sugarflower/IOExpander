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

/*
Currently the software SPI instruction does not work well with PCF 8574.
The circuit may need to be devised, and there may be a bug in this software SPI instruction. :(
*/
void IOExpander::beginSPI(){
	IOExpander::CS =3;
	IOExpander::SCK=2;
	IOExpander::SI =1;
	IOExpander::SO =0;
	IOExpander::pinMode(CS,OUTPUT);
	IOExpander::pinMode(SCK,OUTPUT);
	IOExpander::pinMode(SI,OUTPUT);
	IOExpander::pinMode(SO,INPUT);
}
void IOExpander::setSPIAddress(uint8_t addr){
	IOExpander::spi_addr=addr;
}
void IOExpander::writeSPI(uint16_t addr,uint8_t value){
	IOExpander::spiBuf[0]=IOExpander::spi_addr;
	IOExpander::spiBuf[1]=addr >> 8;
	IOExpander::spiBuf[2]=addr & 0xff;
	IOExpander::spiBuf[3]=value;
	IOExpander::digitalWrite(IOExpander::CS,LOW);
	for(int i0=0; i0<4; i0++){
		for(int i1=0; i1<8; i1++){
			IOExpander::digitalWrite(IOExpander::SCK,LOW);
			IOExpander::digitalWrite(IOExpander::SI,(IOExpander::spiBuf[i0]&(0x80>>i1))!=0 ? HIGH : LOW );
			IOExpander::digitalWrite(IOExpander::SCK,HIGH);
		}
	}
	IOExpander::digitalWrite(IOExpander::CS,HIGH);
}
uint8_t IOExpander::readSPI(uint16_t addr){
	uint8_t ret;
	IOExpander::spiBuf[0]=IOExpander::spi_addr+1;
	IOExpander::spiBuf[1]=addr >> 8;
	IOExpander::spiBuf[2]=addr & 0xff;
	IOExpander::digitalWrite(IOExpander::CS,LOW);
	for(int i0=0; i0<3; i0++){
		for(int i1=0; i1<8; i1++){
			IOExpander::digitalWrite(IOExpander::SCK,LOW);
			IOExpander::digitalWrite(IOExpander::SI,(IOExpander::spiBuf[i0]&(0x80>>i1))!=0 ? HIGH : LOW );
			IOExpander::digitalWrite(IOExpander::SCK,HIGH);
		}
	}
	ret=0;
	for(int i1=0; i1<8; i1++){
		IOExpander::digitalWrite(IOExpander::SCK,LOW);
		ret |= IOExpander::digitalRead(IOExpander::SO)<<(7-i1);
		IOExpander::digitalWrite(IOExpander::SCK,HIGH);
	}
	IOExpander::digitalWrite(IOExpander::CS,HIGH);
	return ret;
}

#ifdef __IOE_PCF8574__
uint8_t IOExpander::readByte(){
	Wire.requestFrom(device_addr,1);
	if( Wire.available() ){
		return Wire.read();
	}
}

void IOExpander::writeByte(uint8_t data){
	Wire.beginTransmission(device_addr);
	Wire.write(data);
	Wire.endTransmission();
}

// pcf8574 hasn't have pinmode this is dummy
void IOExpander::pinMode(uint8_t pin, uint8_t mode){}
#endif


/*
Arduino.h

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
*/
