# EEPROM_Programmer
A simple programmer for 28C16 IC, parallel EEPROM

## Kicad files
All the kicad files including schematic and pcb are good to go
you can make your changes and recreate the gerber files
but a complete set of gerbers are present in the repo.

## Arduino code
The arduino code is in the `arduino_programmer` directory. It has some important functions which you can use to program your EEPROM however you like.
by default it programs the chip from rows 0 to 9 for the digits of a seven-segment-display.

Pay attention to the connections which are stated in the code.
