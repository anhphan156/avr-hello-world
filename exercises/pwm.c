#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

float t = 0; // 0 -> 1

ISR (TIMER0_OVF_vect){
	// set new match location for next period
	OCR0A = t * 255;
}

int main() {
	DDRD |= 1 << 6;

	// fast pwn and set OC0A to BOTTOM when match
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

	// t = 0 initially so the first period is off
	OCR0A = t * 255;

	// interrupt 
	TIMSK0 = 1 << TOIE0;
	sei();

	// prescaler
	TCCR0B = (1 << CS02) | (1 << CS00);

	float dir = 1.0f;
	while(1){
		// increasing match location
		_delay_ms(50);
		t += .1f * dir;
		if(t >= .95f || t <= 0.05f)
			dir *= -1.0f;
	}

	return 0;
}
