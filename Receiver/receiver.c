#include <avr/io.h>
#define F_CPU 1200000
#include <util/delay.h>
#define s_bit(x,y) x |= (1<<y)
#define c_bit(x,y) x &= ~(1<<y)
#define is_set(x,y) x & (1<<y)
#define is_clear(x,y) !(x & (1<<y))
#define True 1
#define False 0
#define up PB2
#define down PB4
#define rf PB3
#define delay_time 5000
#define s_1 23
#define s_2 20
#define s_3 17

void init_FPWM(void);
void get_bits(char* bits);
int main(void)
{
  c_bit(DDRB,rf);
  s_bit(DDRB,up);
  s_bit(DDRB,down);
  s_bit(PORTB,rf);
  c_bit(PORTB,up);
  c_bit(PORTB,down);
  init_FPWM();
  _delay_ms(500);
  OCR0A = s_2;

  unsigned int counter1;
  counter1 = 0;
  unsigned char bits[2];
  bits[0] = 0x01;
  bits[1] = 0;
  while(True)
  {
    if (counter1 > 6000)
    {
      counter1 = 0;
      bits[0] = 0x01;
      c_bit(PORTB,up);
      c_bit(PORTB,down);
      OCR0A = s_2;
    }

    counter1++;
    if (is_set(PINB,rf))
    {
      counter1 = 0;
      bits[0] = 0x01;
      get_bits(bits);
    }
    if((!(bits[0]^0xA5)) && (!((bits[1]&0x0F)^0x0C)))
    {
      {
        if(!((bits[1]&0x30)^0x10))
        {
          OCR0A = s_1;
        }
        else if (!((bits[1]&0x30)^0x20))
        {
          OCR0A = s_3;
        }
        else
        {
          OCR0A = s_2;
        }

        if(!((bits[1]&0xC0)^0x40))
        {
          c_bit(PORTB,down);
          s_bit(PORTB,up);
        }
        else if (!((bits[1]&0xC0)^0x80))
        {
          c_bit(PORTB,up);
          s_bit(PORTB,down);
        }
        else
        {
          c_bit(PORTB,down);
          c_bit(PORTB,up);
        }
      }
    }
  }
  return 0;
}
void init_FPWM(void)
{
  s_bit(DDRB,PB0);

  // Fast PWM Mode
  s_bit(TCCR0A,WGM00);
  s_bit(TCCR0A,WGM01);
  c_bit(TCCR0B,WGM02);

  // FAST PWM TOP --> set, OCR0A --> clear
  s_bit(TCCR0A,COM0A1);
  c_bit(TCCR0A,COM0A0);

  // Timer interrupt mask
  s_bit(TIMSK0,TOIE0);

  OCR0A = s_2;

  // Prescaler --> 64
  s_bit(TCCR0B,CS00);
  s_bit(TCCR0B,CS01);
}
void get_bits(char* bits)
{
  int i,j,k;
  j = 0;
  k = 1;
  _delay_us(100);
  for(i=1;i<16;i++)
  {

    if(k == 8)
    {
      k = 0;
      j++;
    }
    _delay_us(delay_time);
    if(is_set(PINB,rf))
    {
      s_bit(bits[j],k);
      k++;
    }
    else
    {
      c_bit(bits[j],k);
      k++;
    }
  }
}
