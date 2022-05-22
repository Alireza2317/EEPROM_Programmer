// parallel EEPROM Programmer
// 28c16 IC
// written by: github.com/alireza2317
// inspired by Ben Eater


// using 2 shift registers (74HC595) we are gonna put the address on the outputs of them
// 8 bits of data (I/O) can be covered by other arduino pins
// also ~OE and ~WE can be connected to the arduino directly

// shift register pins
#define SHIFT_CLK 2
#define SHIFT_LATCH 3
#define SHIFT_DATA 4


// EEPROM pins
#define OE A0 // active low
#define WE A1 // active low

#define D0 5
#define D1 6
#define D2 7
#define D3 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12


#define NUMBER_OF_ROWS_OF_EEPROM 2048


const byte data_pins[8] = {D0, D1, D2, D3, D4, D5, D6, D7};
 
void set_address(const unsigned int address) {
	// 8 bits at a time
	// 3 MS bits of the address
	shiftOut(SHIFT_DATA, SHIFT_CLK, LSBFIRST, address >> 8);
	// 8 LS remaining bits by default 
	shiftOut(SHIFT_DATA, SHIFT_CLK, LSBFIRST, address);
}

byte read_EEPROM(const unsigned int address) {
	set_address(address);
	
	// enabling the output of the EEPROM
	digitalWrite(OE, LOW);
	
	// setting the data pins as INPUT, because data will be read from them
	for (byte i=0; i<8; i++) {
		pinMode(data_pins[i], INPUT);
	}

	byte data = 0;
	for (byte i=7; i>=0; i--) {
		data = (data << 1) + digitalRead(data_pins[i]);
	}

	// disable the output enable pin
	digitalWrite(OE, HIGH);

	return data;
}

void show_EEPROM_content() {
	for (unsigned int base=0; base<NUMBER_OF_ROWS_OF_EEPROM; base+=16) {
		byte data[16];
		for (byte offset=0; offset<16; offset++) {
			data[offset] = read_EEPROM(base + offset);
		}
		
		char buffer[90];
		sprintf(
			buffer,
			"%03x: %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x", 
			base, 
			data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
			data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]
		);

		Serial.println("     00 01 02 03 04 05 06 07   08 09 10 11 12 13 14 15");
		Serial.println(buffer);
	}
}

void write_EEPROM(const unsigned int address, byte data) {
	// disabling the output enable, because we want to write data to the chip
	digitalWrite(OE, HIGH);

	// setting the data pins as OUTPUT, because data will be sent to them
	for (byte i=0; i<8; i++) {
		pinMode(data_pins[i], OUTPUT);
	}

	for (byte i=0; i<8; i++) {
		digitalWrite(data_pins[i], data & 1);
		// shifting the data to right, so each time we can get the LSB by & 1
		data >>= 1;
	}
	digitalWrite(WE, HIGH);
	digitalWrite(WE, LOW);
	delayMicroseconds(1);
	digitalWrite(WE, HIGH);
	delay(15);
}

void erase_EEPROM() {
	for (unsigned int address=0; address<NUMBER_OF_ROWS_OF_EEPROM; address++) {
		write_EEPROM(address, 0xff);
	}
}


void setup() {
	// setting pin modes
	pinMode(SHIFT_CLK, OUTPUT);
	pinMode(SHIFT_LATCH, OUTPUT);
	pinMode(SHIFT_DATA, OUTPUT);
	pinMode(OE, OUTPUT);
	
	// the order of these two lines is correct as is
	// because if the pin is set to OUTPUT before setting it HIGH
	// it may go LOW and may accidentally write something to the chip
	digitalWrite(WE, HIGH);
	pinMode(WE, OUTPUT);

	Serial.begin(57600);

	// seven segment display(common cathode) data to decode it properly
	byte digits[10] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b};

	// seven segment display(common anode) data to decode it properly
	//byte digits[10] = {0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84};

	for (byte value=0; value<10; value++) {
		write_EEPROM(value, digits[value]);
	}

}

void loop() {
	// nothing
}