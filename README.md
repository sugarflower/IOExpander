** Unique feature.
  You can access SPI devices (such as SPI SRAM) via I / O Expander connected to arduino or gamebuino (MAKERbuino).

** Available I / O Expander
-PCF 8574 N / PCF 8574 A
-MCP23017

  In the future also MCP 23008.

important point.
  In PCF 8574, input from SPI device is not good. It may be possible depending on the circuit, but it is easier to use the MCP 23017 that can set the PIN mode.
