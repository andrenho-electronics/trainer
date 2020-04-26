#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int freq = 7;
volatile bool clk = false;
volatile bool invert = false;

static void
update_freq(int i)
{
  if (i == 1) {
    ++freq;
    if (freq > 7)
      freq = 0;
  } else if (i == -1) {
    --freq;
    if (freq < 0)
      freq = 7;
  }


  if (freq == 7) {
    PORTB = (PORTB & _BV(PB7)) | 0b1111111;
    PORTD &= ~_BV(PD5);   // clear output
  } else {
    PORTB = (PORTB & _BV(PB7)) | ~_BV(freq);
    PORTD |= _BV(PD5);    // set output
    clk = true;
  }

  // TIMER 1 for interrupt frequency 2 Hz:
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // turn on CTC mode
  TCCR1B |= _BV(WGM12);
  switch (freq) {
    case 0:  // 0.5 Hz
      OCR1A = 15624;
      TCCR1B |= (0 << CS12) | _BV(CS11) | _BV(CS10);
      break;
    case 1:  // 1 Hz
      OCR1A = 62499;
      TCCR1B |= (0 << CS12) | _BV(CS11) | (0 << CS10);
      break;
    case 2:  // 2 Hz
      OCR1A = 31249;
      TCCR1B |= (0 << CS12) | _BV(CS11) | (0 << CS10);
      break;
    case 3:  // 5 Hz
      OCR1A = 12499;
      TCCR1B |= (0 << CS12) | _BV(CS11) | (0 << CS10);
      break;
    case 4:  // 10 Hz
      OCR1A = 49999;
      TCCR1B |= (0 << CS12) | (0 << CS11) | _BV(CS10);
      break;
    case 5:  // 100 Hz
      OCR1A = 4999;
      TCCR1B |= (0 << CS12) | (0 << CS11) | _BV(CS10);
      break;
    case 6:  // 1 kHz
      OCR1A = 499;
      TCCR1B |= (0 << CS12) | (0 << CS11) | _BV(CS10);
      break;
    case 7:
      OCR1A = 0;
      TCCR1B = 0;
      break;
  }
  // enable timer compare interrupt
  TIMSK |= _BV(OCIE1A);
  sei(); // allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (clk)
    PORTD &= ~_BV(PD5);
  else
    PORTD |= _BV(PD5);
	clk = !clk;
}

void
check_next_prev()
{
  if ((PINB & _BV(PINB7)) == 0) {
    update_freq(1);
    _delay_ms(40);
    while ((PINB & _BV(PINB7)) == 0);
    _delay_ms(40);
  } else if ((PIND & _BV(PIND6)) == 0) {
    update_freq(-1);
    _delay_ms(40);
    while ((PIND & _BV(PIND6)) == 0);
    _delay_ms(40);
  }
}

void
blink(int pin)
{
  if (invert) {
    PORTD &= ~_BV(pin);
    _delay_ms(60);
    PORTD |= _BV(pin);
  } else {
    PORTD |= _BV(pin);
    _delay_ms(60);
    PORTD &= ~_BV(pin);
  }
}

void
check_debounce()
{
  if ((PIND & _BV(PIND0)) == 0) {
    blink(PD2);
    while ((PIND & _BV(PIND0)) == 0);
    _delay_ms(120);
  } else if ((PIND & _BV(PIND1)) == 0) {
    blink(PD3);
    while ((PIND & _BV(PIND1)) == 0);
    _delay_ms(120);
  } else if ((PINA & _BV(PINA1)) == 0) {
    blink(PD4);
    while ((PINA & _BV(PINA1)) == 0);
    _delay_ms(120);
  }
}

void
check_debounce_invert()
{
  if ((PINA & _BV(PINA0)) == 0) {
    invert = true;
    PORTD |= _BV(PD2);
    PORTD |= _BV(PD3);
    PORTD |= _BV(PD4);
  } else {
    invert = false;
    PORTD &= ~_BV(PD2);
    PORTD &= ~_BV(PD3);
    PORTD &= ~_BV(PD4);
  }
}

int
main()
{
  DDRB  = 0b01111111;    // all inputs
  PORTB = 0b11111111;    // all indicator leds off
  //        |'''''''  leds
  //        '- input (next)

  DDRD  = 0b00011100;
  PORTD = 0b01100011;
  //         ||||||'- input push A
  //         |||||'-- input push B
  //         ||'''--- push outputs
  //         |'------ clock output
  //         '------- input (prev)
  
  DDRA  = 0b00;
  PORTA = 0b11;
  //        |'- input invert push
  //        '-- input push C

  update_freq(0);

  for (;;) {
    check_next_prev();
    check_debounce_invert();
    check_debounce();
  }
}
