#include "IOExpander.h"

void IOExpander::begin(){
	Wire.begin();
	IOExpander::spi_mode = false;
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

	if( !IOExpander::spi_mode){
		Wire.beginTransmission(device_addr);
		Wire.write(0x13);
		Wire.endTransmission();
		Wire.requestFrom(device_addr,1);
		while( Wire.available() ){
			IOExpander::buf = Wire.read();
		}
		IOExpander::buf = IOExpander::buf << 8;
	}
	
	Wire.beginTransmission(device_addr);
	Wire.write(0x12);
	Wire.endTransmission();
	Wire.requestFrom(device_addr,1);
	while( Wire.available() ){
		IOExpander::buf = (IOExpander::buf & 0xff00) | Wire.read();
	}
	return IOExpander::buf;
}

void IOExpander::writeByte(uint16_t data){
	Wire.beginTransmission(device_addr);
	Wire.write(0x14);
	Wire.write(data & 0xff);
	Wire.endTransmission();	
	if(!IOExpander::spi_mode){
		Wire.beginTransmission(device_addr);
		Wire.write(0x15);
		Wire.write((data>>8) & 0xff);
		Wire.endTransmission();
	}
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
	while( Wire.available() ){
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
			Wire.write(port);
			Wire.write(IOExpander::buf);
			Wire.endTransmission();

			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.endTransmission();
			Wire.requestFrom(device_addr,1);
			while( Wire.available() ){
				IOExpander::buf=Wire.read();
			}
			IOExpander::buf &= ~( 1 << pin ); // reset bit
			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.write(IOExpander::buf);

			break;
		case INPUT_PULLUP:
			Wire.write(port);
			Wire.write(IOExpander::buf);
			Wire.endTransmission();

			Wire.beginTransmission(device_addr);
			Wire.write(0x0c + port);
			Wire.endTransmission();
			Wire.requestFrom(device_addr,1);
			while( Wire.available() ){
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

/*
 software spi via iic ( pretty slowly )

*/
void IOExpander::beginSPI(){
	IOExpander::SCK=2;
	IOExpander::SI =1;
	IOExpander::SO =0;
	IOExpander::pinMode(IOExpander::SCK,OUTPUT);
	IOExpander::pinMode(IOExpander::SI,OUTPUT);
	IOExpander::pinMode(IOExpander::SO,INPUT);
	IOExpander::digitalWrite(IOExpander::SCK,LOW);
}

void IOExpander::spiWriteByte(uint8_t value){
	IOExpander::spi_mode = true;
	for(int i=0; i<8; i++){
		IOExpander::digitalWrite(IOExpander::SI,(value&(0x80>>i))!=0 ? HIGH : LOW );
		IOExpander::digitalWrite(IOExpander::SCK,HIGH);
		IOExpander::digitalWrite(IOExpander::SCK,LOW);
	}
	IOExpander::spi_mode = false;
}

uint8_t IOExpander::spiReadByte(){
	IOExpander::spi_mode = true;
	uint8_t ret=0;
	for(int i=0; i<8; i++){
		
		ret |= IOExpander::digitalRead(IOExpander::SO)<<(7-i);
		IOExpander::digitalWrite(IOExpander::SCK,HIGH);
		IOExpander::digitalWrite(IOExpander::SCK,LOW);
	}
	IOExpander::spi_mode = false;
	return ret;
}

