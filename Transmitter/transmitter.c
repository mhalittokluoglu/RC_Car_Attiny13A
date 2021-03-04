#include <avr/io.h>
#define F_CPU 1200000
#include <util/delay.h>
#define s_bit(x,y) x |= (1<<y)
#define c_bit(x,y) x &= ~(1<<y)
#define is_set(x,y) x & (1<<y)
#define is_clear(x,y) !(x & (1<<y))
#define True 1
#define False 0
#define Right PB1
#define Left PB2
#define Up PB3
#define Down PB4
#define rf PB0
#define delay_time 5000

void send_zero(void);
void send_one(void);
void init_sender(void);
void init_timer(void);
void stop_timer(void);
void enable_timer(void);
int main(void)
{
  unsigned char is_pressed = False;
  c_bit(DDRB,Right); // Right - PB1 pini input
  c_bit(DDRB,Left); // Left - PB2 pini input
  c_bit(DDRB,Up); // Up - PB3 pini input
  c_bit(DDRB,Down); // Down - PB4 pini input

  s_bit(PORTB,Right); // Right - PB1 pini internal pull up
  s_bit(PORTB,Left); // Left - PB2 pini internal pull up
  s_bit(PORTB,Up); // Up - PB3 pini internal pull up
  s_bit(PORTB,Down); // Down - PB4 pini internal pull up

  init_timer();
  stop_timer();

  while(True)
  {
    if((is_clear(PINB,Right))|(is_clear(PINB,Left))|(is_clear(PINB,Up))|(is_clear(PINB,Down)))
    {
      is_pressed = True;
    }
    else
    {
      is_pressed = False;
      stop_timer();
    }
    if(is_pressed)
    {
      init_sender();
      if (is_clear(PINB,Right))
      {
        send_one();
        send_zero();
      }
      else if (is_clear(PINB,Left))
      {
        send_zero();
        send_one();
      }
      else
      {
        send_zero();
        send_zero();
      }
      if (is_clear(PINB,Up))
      {
        send_one();
        send_zero();
      }
      else if (is_clear(PINB,Down))
      {
        send_zero();
        send_one();
        stop_timer();
      }
      else
      {
        send_zero();
        send_zero();
      }
      _delay_ms(120);
    }
  }

  return 0;
}
void send_zero(void)
{
  stop_timer();
  _delay_us(delay_time);
}
void send_one(void)
{
  enable_timer();
  _delay_us(delay_time);
}
void init_sender(void)
{
  // 1010 0101 0011
  //  A     5    3
  // A53
  send_one();
  send_zero();
  send_one();
  send_zero();
  send_zero();
  send_one();
  send_zero();
  send_one();
  send_zero();
  send_zero();
  send_one();
  send_one();
}
void init_timer(void)
{
  s_bit(DDRB,rf); // rf'yi output olarak kullan

  // CTC modu belirt
  c_bit(TCCR0A,WGM00);
  s_bit(TCCR0A,WGM01);
  c_bit(TCCR0B,WGM02);

  // Toggle
  c_bit(TCCR0A,COM0A1);
  s_bit(TCCR0A,COM0A0);

  // Timer interrupt mask
  s_bit(TIMSK0,TOIE0);

  OCR0A = 15;

  // Prescaler --> 1
  c_bit(TCCR0B,CS02);
  c_bit(TCCR0B,CS01);
  s_bit(TCCR0B,CS00);
}
void stop_timer(void)
{
  c_bit(TCCR0A,COM0A1);
  c_bit(TCCR0A,COM0A0);
  c_bit(PORTB,rf);
}
void enable_timer(void)
{
  c_bit(TCCR0A,COM0A1);
  s_bit(TCCR0A,COM0A0);
}
