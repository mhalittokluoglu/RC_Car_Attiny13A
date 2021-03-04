default: receiver.hex receiver.elf


receiver.hex: receiver.c receiver.elf
	avr-objcopy -O ihex -R .eeprom receiver.elf receiver.hex

receiver.elf: receiver.c
	avr-gcc -g -Os -mmcu=attiny13a -o receiver.elf receiver.c

clean:
	rm -rf receiver.elf receiver.hex

flash:
	avrdude -p attiny13 -c usbasp -U flash:w:receiver.hex:i -F -P usb
