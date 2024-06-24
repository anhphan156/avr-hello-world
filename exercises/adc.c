#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void pwm_setup();
void adc_start_conversion();
void adc_setup();

float t = 0;

int main() {

	adc_setup();
	pwm_setup();

	while(1);

	return 0;
}

ISR (ADC_vect){
	t = ADC / 4;
	adc_start_conversion();
}

ISR (TIMER0_OVF_vect){
	// set new match location for next period
	OCR0A = t;
}

void adc_setup(){
	ADMUX = (1 << REFS0) | (1 << MUX2) | (1 << MUX0);
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	DIDR0 = 1 << ADC5D;

	adc_start_conversion();
}
void pwm_setup(){
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
}
void adc_start_conversion(){
	ADCSRA |= 1 << ADSC;
}
