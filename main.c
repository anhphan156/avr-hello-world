#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BLINK_MS 5000
#define PRESCALER 10
#define CLOCK_RATE 15998976
#define COUNTER_VALUE (CLOCK_RATE >> PRESCALER)

ISR(TIMER1_COMPA_vect){
	PORTB ^= _BV(PORTB5);
}

int main() {
  /*DDRB |= 0b00100000;*/
  DDRB |= _BV(PORTB5);

	OCR1A = COUNTER_VALUE;

	TCCR1A = 0x00;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10) | (1 << CS12);

	TIMSK1 |= (1 << OCIE1A);

	sei();

  while (1);

  return 0;
}
