#include <avr/io.h>

#define mm(x) ((volatile void*)x)

int main() {
  volatile unsigned char* ddrd = mm(0x2A);
  volatile unsigned char* portd = mm(0x2B);
  volatile unsigned char* pind = mm(0x29);

	char switchState = 0;

	*ddrd |= (1 << DDD3) | (1 << DDD4) | (1 << DDD5);

	while(1){
		if (*pind & (1 << PIND2))
			switchState = 1;
		else
			switchState = 0;

		if(switchState){
			*portd = (0 << PORTD3) | (1 << PORTD4) | (1 << PORTD5);
		}else{
			*portd = (1 << PORTD3) | (0 << PORTD4) | (0 << PORTD5);
		}
	}

  return 0;
}
