#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define mmio(x) ((volatile void*)x)

ISR (TIMER1_OVF_vect){
	volatile unsigned char *portd = mmio(0x2B);
	volatile unsigned short *tcnt1 = mmio(0x84);
	*portd ^= 1 << PORTD2;

	*tcnt1 = 65535 - (F_CPU/1024) / 2;
}

int main() {

	volatile unsigned char *ddrd = mmio(0x2A);
	volatile unsigned short *tcnt1 = mmio(0x84);
	volatile unsigned char *tccr1a = mmio(0x80);
	volatile unsigned char *tccr1b = mmio(0x81);
	volatile unsigned char *timsk1 = mmio(0x6F);

	*ddrd |= 1 << DDD2;

	*tcnt1 = 65535 - (F_CPU/1024) / 2;

	*tccr1a = 0x00;
	// 16MHz/1024 = 15625Hz
	*tccr1b |= (1 << CS10) | (1 << CS12);

	*timsk1 |= (1 << TOIE1);

	sei();

  while (1);

  return 0;
}
