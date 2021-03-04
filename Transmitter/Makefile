default: transmitter.hex transmitter.elf


transmitter.hex: transmitter.c transmitter.elf
	avr-objcopy -O ihex -R .eeprom transmitter.elf transmitter.hex

transmitter.elf: transmitter.c
	avr-gcc -g -Os -mmcu=attiny13a -o transmitter.elf transmitter.c

clean:
	rm -rf transmitter.elf transmitter.hex

flash:
	avrdude -p attiny13 -c usbasp -U flash:w:transmitter.hex:i -F -P usb
