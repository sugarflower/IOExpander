#include <Gamebuino.h>
#include <IOExpander.h>
Gamebuino gb;
IOExpander ex;

/*
 CIRCUIT

 buino     MCP23017   23LC512(64K SRAM)
---------------------------------------------
 VCC  -+-> VCC   ---> VCC --> R10k --> HOLD
       +-> RESET
 GND  -+-> GND   ---> GND
       +-> A0
       +-> A1
       +-> A2
 SDA  -->  SDA
 SCL  -->  SCL
           PA0    ---> SO
	   PA1    ---> SI
	   PA2    ---> SCK
	   PA3    ---> CS

   NOTE)
    run really slowly.
    need optimaize :(
*/

int a;

void setup() {
	gb.begin();
	gb.titleScreen(F("SRAM_TEST"));
	gb.pickRandomSeed();
	gb.frameCount = LCDWIDTH*8;
	gb.setFrameRate(30);

	Serial.begin(9600);

	ex.begin(); // IOExpander begin
	ex.setDeviceAddress(0x20); //I2C IOExpander device address
	ex.beginSPI(); // SPI via I2C begin
	
	ex.pinMode(3,OUTPUT); //CS
	ex.digitalWrite(3,HIGH); //CS high
}

void loop() {
	if(gb.update()){
		if(gb.buttons.pressed(BTN_A)){
			Serial.println(F("write Value"));
      			for(int i=0; i<10; i++){
				writeSRAM(i, a);
				Serial.println(a);
				a++;
			}
			gb.popup(F("write to sram"),50);
		}
      		if(gb.buttons.pressed(BTN_B)){
			Serial.println(F("read Value"));
      			for(int i=0; i<10; i++){
				Serial.println(readSRAM(i));
			}
			gb.popup(F("read from sram"),50);
		}
      		
		if(gb.buttons.pressed(BTN_C)){
	       	 	gb.titleScreen(F("SRAM_TEST"));
      		}
	}
}

void writeSRAM(uint16_t address, uint8_t value){
	ex.digitalWrite(3,LOW);
	ex.spiWriteByte(0x02);
	ex.spiWriteByte(address >> 8);
	ex.spiWriteByte(address & 0xff);
	ex.spiWriteByte(value);
	ex.digitalWrite(3,HIGH);
}

uint8_t readSRAM(uint16_t address){
	uint8_t value;
	ex.digitalWrite(3,LOW);
	ex.spiWriteByte(0x03);
	ex.spiWriteByte(address >> 8);
	ex.spiWriteByte(address & 0xff);
	ex.spiWriteByte(0);
	value=ex.spiReadByte();
	ex.digitalWrite(3,HIGH);
	return value;
}
