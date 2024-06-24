CC=avr-gcc
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega328p

all:out

USBPORT:=/dev/ttyACM0

out: main.o other.o
	$(CC) $(CFLAGS) main.o other.o -o $@

main.o: main.c
	$(CC) $(CFLAGS) -g -c -o main.o main.c	

other.o: ./exercises/lcd.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

out.hex: out
	avr-objcopy -O ihex -R .eeprom $< $@

install: out.hex
	#avrdude -F -V -c arduino -p ATMEGA328p -P ${USBPORT} -b 115200 -U flash:w:$<
	avrdude -c stk500v1 -p ATMEGA328p -P ${USBPORT} -b 19200 -U flash:w:$<

clean:
	rm *.o out
