#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define EN PORTB0
#define RS PORTB1
#define D4 PORTB2
#define D5 PORTB3
#define D6 PORTB4
#define D7 PORTB5

#define ENABLE \
	PORTB |= 1 << EN;\
	_delay_ms(100);\
	PORTB &= ~(1 << EN);\
	_delay_ms(100);

void pwm_setup();
void adc_start_conversion();
void adc_setup();
void lcd_setup();
void lcd_write();

float t = 0;

int _main() {

	/*adc_setup();*/
	/*pwm_setup();*/
	lcd_setup();
	_delay_ms(0.1f);
	lcd_write();

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

void lcd_write(){
	DDRB = 0b00111111;

	PORTB = 1 << RS; // RS = 1 to start writing characters

	// H
	PORTB |= 1 << D6;
	ENABLE;
	PORTB &= ~(1 << D6);
	PORTB |= 1 << D7;
	ENABLE;
	PORTB &= ~(1 << D7);

	// I
	PORTB |= 1 << D6;
	ENABLE;
	PORTB &= ~(1 << D6);
	PORTB |= 1 << D4;
	PORTB |= 1 << D7;
	ENABLE;
	PORTB &= ~(1 << D4);
	PORTB &= ~(1 << D7);
	
}

void lcd_setup(){
	DDRB = 0b00111111;

	PORTB = 0 << RS; // RS = 0 for setup mode

	// set 4 bit mode
	PORTB |= 1 << D5;
	ENABLE;
	PORTB &= ~(1 << D5);

	// clear display
	ENABLE;
	PORTB |= 1 << D4;
	ENABLE;
	PORTB &= ~(1 << D4);

	// return home
	ENABLE;
	PORTB |= 1 << D5;
	ENABLE;
	PORTB &= ~(1 << D5);

	// display on
	ENABLE;
	PORTB |= 1 << D6;
	PORTB |= 1 << D7;
	ENABLE;
	PORTB &= ~(1 << D6);
	PORTB &= ~(1 << D7);
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
