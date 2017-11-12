#include <Gamebuino.h>
#include <EEPROM.h>
#include <IOExpander.h>
Gamebuino gb;
IOExpander ex;

/*
i2c i/o expander input and output example

CIRCUIT

buino / mcp23017 or pcf8574
vcc ---> vcc (reset)
gnd ---> gnd
sda ---> sda
scl ---> scl
         pa0/p0 --> R1K --> LED --> GND
	 pa1/p1 --> SW --> GND
*/

void setup() {
	gb.begin();
	gb.titleScreen(F("IN_OUT"));
	gb.pickRandomSeed();
	gb.frameCount = LCDWIDTH*8;
	gb.setFrameRate(30);

	ex.begin();
	ex.setDeviceAddress(0x20);
	
	// NOTE) pinMode does not work with pcf8574
	ex.pinMode(0,OUTPUT);
	ex.pinMode(1,INPUT_PULLUP);
}

void loop() {
	if(gb.update()){
		gb.display.print("p1 value:");
		gb.display.println(ex.digitalRead(1));
		if(gb.buttons.pressed(BTN_A)){
			ex.digitalWrite(0,HIGH); // LED ON 
      		}
      		if(gb.buttons.pressed(BTN_B)){
			ex.digitalWrite(0,LOW); // LED OFF
      		}
      		
		if(gb.buttons.pressed(BTN_C)){
	       	 	gb.titleScreen(F("IN_OUT"));
      		}
	}
}
